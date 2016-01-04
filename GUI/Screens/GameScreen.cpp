#include "GameScreen.h"
#include "../../NTTTPlayerIce.h"
#include "../../NTTTPlayerMike.h"
#include <string>
#include <algorithm>
#include <fstream>

static int manageGameStatic(void* data)
{
	GameScreen *pGameScreen = static_cast<GameScreen *> (data);
	return pGameScreen->manageGame();
}

void GameScreen::prepareForQuit(){
	if (!m_isThreadProperlyClosed){
		int status;
		SDL_WaitThread(m_gameThread, &status);
		std::cout << "Game ended: " << status << std::endl;
	}
}

void GameScreen::writeLog(const int winner) const{
	//TODO

	std::ofstream outputFile;
	outputFile.open("logs/" + *m_logName);

	if (!outputFile.good()){
		std::cout << "Failed to open logs/" << *m_logName << std::endl;
		outputFile.close();
		return;
	}

	outputFile << "# Players: " << std::endl;
	outputFile << "Player 1: " << m_player1->getName() << std::endl;
	outputFile << "Player 2: " << m_player2->getName() << std::endl;
	outputFile << "# Settings:" << std::endl;
	outputFile << "BoardCount: " << *m_boardCount << std::endl;
	outputFile << "BoardSize: " << *m_boardSize << std::endl;
	outputFile << "LineSize: " << *m_lineSize << std::endl;
	outputFile << "Game Seed: " << *m_gameSeed << std::endl;
	outputFile << "# Moves:" << std::endl;
	for (unsigned int index = 0; index < m_moves.size(); index++){
		outputFile << m_moves[index].getBoardNumber() << " : (" << m_moves[index].getSquareX() << ", " << m_moves[index].getSquareY() << ")" << std::endl;
	}
	outputFile << "# Winner:" << std::endl;
	outputFile << "Winner: " << winner << std::endl;

	outputFile.close();

}

int GameScreen::manageGame(){
	int player = 1;
	NTTTMove move(0, 0, 0);
	bool gameActive = true;
	while (!*m_quit && gameActive)
	{

		while (m_moves.size() != getMovesCount())
			SDL_Delay(5);

		if (gameActive)
		{
			srand(*m_gameSeed); //Sets the seed before every move
			switch (player)
			{
			case 1:
				move = m_player1->performMove(*m_game);
				m_game->makeMove(move, NTTTBoard::RED);
				break;
			case 2:
				move = m_player2->performMove(*m_game);
				m_game->makeMove(move, NTTTBoard::BLUE);
				break;
			}

			m_moves.push_back(move);

			player = 3 - player;

			if (!m_game->isActive())
			{
				std::cout << "Player " << player << " won!" << std::endl;
				gameActive = false;
				m_winner = player;
				SDL_Delay(3000);
			}
			if (*m_manualMode){
				do{
					SDL_Delay(100);
					if (m_backward){
						if (m_moves.size() <= 1)
							continue;
						m_game->undoMove(m_moves[m_moves.size() - 1]);
						m_moves.pop_back();
						player = 3 - player;
						m_backward = false;
					}
				} while (!m_forward && !*m_quit);
				m_forward = false;
			}
			else{
				SDL_Delay(1000);
			}
		}
	}

	
	if (*m_writeLog)
		writeLog(m_winner);
	

	//m_isGameThreadRunning must be set to false just before the end.
	m_isGameThreadRunning = false;

	return player;
}

GameScreen::GameScreen(GameData data){
	m_currentState = data.currentState;

	m_game = data.game;
	m_movesFont = data.movesFont;

	m_boardCount = data.boardCount;
	m_boardSize = data.boardSize;
	m_lineSize = data.lineSize;
	m_gameSeed = data.gameSeed;
	m_manualMode = data.manualMode;
	m_writeLog = data.writeLog;
	m_logName = data.logName;

	m_quit = data.quit;

	m_redCross = data.redCross;
	m_blueCross = data.blueCross;
}

GameScreen::~GameScreen(){
	cleanUp();
}

void GameScreen::update(SDL_Renderer *renderer) {
	if (!m_isInitialized){
		initialize(renderer);
		m_isInitialized = true;
	}
	else if (!m_isGameThreadRunning && !m_isThreadProperlyClosed){
		int status;
		SDL_WaitThread(m_gameThread, &status);
		std::cout << "Game ended: " << status << std::endl;
		m_isInitialized = false;
		m_isThreadProperlyClosed = true;

		(*m_currentState) = MAIN_MENU;
	}
	if (m_winner != 0 && m_winnerDisplayText == nullptr){
		initializeWinnerDisplayText(renderer);
	}
	for (unsigned int index = getMovesCount(); index < m_moves.size(); index++){
		addMove(renderer, m_moves[index]);
	}
	for (unsigned int index = getMovesCount(); index > m_moves.size(); index--){
		removeMove();
	}
}

void GameScreen::removeMove(){
	if (m_excessMoves > 0){
		m_excessMoves--;
		return;
	}
	if (m_movesTexts.size() == 0)
		return;
	delete m_movesTexts[m_movesTexts.size() - 1];
	m_movesTexts.resize(m_movesTexts.size() - 1);
}

void GameScreen::addMove(SDL_Renderer *renderer, NTTTMove move){
	if (m_movesTexts.size() >= m_maxMoves){
		m_excessMoves++;
		return;
	}
	const int x = m_playingFieldSize;

	Text *text = new Text(renderer, std::to_string(move.getBoardNumber()) + " : (" + std::to_string(move.getSquareX())
		+ ", " + std::to_string(move.getSquareY()) + ")", m_movesFont, { 255, 255, 255 }, 0,
		0);

	text->set(x + 2 * PADDING_X + (int)(m_movesTexts.size() / m_amountInColumn) * m_movesWidth + m_movesWidth / 2 - text->getWidth() / 2,
		m_winnerText->getY() + m_winnerText->getHeight() + 2 * PADDING_Y + (m_movesTexts.size() % m_amountInColumn) * m_movesHeight + m_movesHeight / 2 - text->getHeight() / 2);

	m_movesTexts.push_back(text);

}

void GameScreen::initializeWinnerDisplayText(SDL_Renderer *renderer){
	const int x = m_playingFieldSize;
	const int width = m_width - x;
	switch (m_winner){
	case 1:
		m_winnerDisplayText = new Text(renderer, m_player1->getName(), m_guiFont, { 255, 0, 0 }, x, m_lineSizeText->getY() + m_lineSizeText->getHeight() + PADDING_Y);
		m_winnerDisplayText->setX(x + width - PADDING_X - m_winnerDisplayText->getWidth());
		break;
	case 2:
		m_winnerDisplayText = new Text(renderer, m_player2->getName(), m_guiFont, { 0, 0, 255 }, x, m_lineSizeText->getY() + m_lineSizeText->getHeight() + PADDING_Y);
		m_winnerDisplayText->setX(x + width - PADDING_X - m_winnerDisplayText->getWidth());
		break;
	}
}

void GameScreen::initialize(SDL_Renderer *renderer){
	cleanUp();

	Texture texture(renderer, "0", m_movesFont, { 0, 0, 0, 255 });

	m_movesHeight = texture.getHeight() + 2 * PADDING_Y;


	//Maybe temporary ----->
	m_game->NewGame(*m_boardCount, *m_boardSize, *m_lineSize);

	srand(*m_gameSeed);

	NTTTPlayerIce *ice = new NTTTPlayerIce();
	NTTTPlayerMike *mike = new NTTTPlayerMike();

	NTTTPlayer::OrderChoice iceOrderChoice, mikeOrderChoice;
	iceOrderChoice = ice->chooseOrder(*m_game);
	mikeOrderChoice = mike->chooseOrder(*m_game);

	if (iceOrderChoice == mikeOrderChoice){
		if (rand() % 2 == 0){
			std::cout << "Ice starts" << std::endl;
			m_player1 = ice;
			m_player2 = mike;
		}
		else {
			std::cout << "Mike starts" << std::endl;
			m_player1 = mike;
			m_player2 = ice;
		}
	}
	else if (iceOrderChoice == NTTTPlayer::FIRST || mikeOrderChoice == NTTTPlayer::LAST){
		std::cout << "Ice starts" << std::endl;
		m_player1 = ice;
		m_player2 = mike;
	}
	else{
		std::cout << "Mike starts" << std::endl;
		m_player1 = mike;
		m_player2 = ice;
	}

	m_gridSize = (int)ceil(sqrt(*m_boardCount));
	m_boardRenderSize = (m_playingFieldSize - (1 + m_gridSize) * BOARD_PADDING) / m_gridSize;
	//<-----

	const int x = m_playingFieldSize;
	const int y = 0;
	const int width = m_width - x;
	const SDL_Color black = { 0, 0, 0, 255 };

	m_vsText = new Text(renderer, " vs ", m_headlineFont, { 0, 0, 0 }, x, y);
	m_vsText->set(x - m_vsText->getWidth() / 2 + width / 2, PADDING_Y);

	m_player1Text = new Text(renderer, m_player1->getName(), m_headlineFont, { 255, 0, 0 }, x, m_vsText->getY());
	m_player2Text = new Text(renderer, m_player2->getName(), m_headlineFont, { 0, 0, 255 }, m_vsText->getX() + m_vsText->getWidth(), m_vsText->getY());
	m_player1Text->setX(m_vsText->getX() - m_player1Text->getWidth());

	m_boardCountText = new Text(renderer, "BoardCount: " + std::to_string(*m_boardCount), m_guiFont, black, x, m_vsText->getY() + m_vsText->getHeight() + PADDING_Y * 2);
	m_boardCountText->setX(x + width / 2 - m_boardCountText->getWidth() / 2);

	m_boardSizeText = new Text(renderer, "BoardSize: " + std::to_string(*m_boardSize), m_guiFont, black, x, m_boardCountText->getY() + m_boardCountText->getHeight() + PADDING_Y);
	m_boardSizeText->setX(x + width / 2 - m_boardSizeText->getWidth() / 2);

	m_lineSizeText = new Text(renderer, "LineSize: " + std::to_string(*m_lineSize), m_guiFont, black, x, m_boardSizeText->getY() + m_boardSizeText->getHeight() + PADDING_Y);
	m_lineSizeText->setX(x + width / 2 - m_lineSizeText->getWidth() / 2);

	m_winnerText = new Text(renderer, "Winner: ", m_guiFont, black, x + PADDING_X, m_lineSizeText->getY() + m_lineSizeText->getHeight() + PADDING_Y);

	int w, h;

	TTF_SizeText(m_movesFont, (std::to_string(*m_boardCount) + "  : (8,8)").c_str(), &w, &h);

	m_movesWidth = w + PADDING_X * 2; //Estimate

	m_amountInColumn = (int)((m_height - (m_winnerText->getY() + m_winnerText->getHeight() + 3 * PADDING_Y)) / m_movesHeight);
	m_amountInRow = (int)((width - PADDING_X * 2) / m_movesWidth);
	m_maxMoves = m_amountInColumn * m_amountInRow;

	m_moves.clear();
	m_isThreadProperlyClosed = false;
	m_isGameThreadRunning = true;
	m_gameThread = SDL_CreateThread(manageGameStatic, "Game Thread", this);
	if (m_gameThread == NULL){
		std::cout << "Failed to create thread: " << SDL_GetError() << std::endl;
		m_isGameThreadRunning = false;
	}
}

void GameScreen::init(SDL_Renderer *renderer, TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height){
	//This function is almost empty since the gui depends on the settings (the settings has not yet been chosen)
	m_playingFieldSize = std::min(width, height);
	m_headlineFont = headlineFont;
	m_guiFont = guiFont;
	m_width = width;
	m_height = height;
}

void GameScreen::render(SDL_Renderer* renderer) const{
	
	if (!m_isInitialized)
		return;

	for (int index = 0; index < *m_boardCount; index++){

		int boardX = BOARD_PADDING * (index % m_gridSize + 1) + m_boardRenderSize * (index % m_gridSize);
		int boardY = BOARD_PADDING * (int)(index / m_gridSize + 1) + m_boardRenderSize * (int)(index / m_gridSize);
		int squareSize = m_boardRenderSize / *m_boardSize;

		if (m_moves.size() > 0){
			const NTTTMove& move = m_moves[m_moves.size() - 1];
			if (move.getBoardNumber() == index){
				SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255); //Color of the "last move" square
				SDL_Rect rect = { boardX + squareSize * move.getSquareX(), boardY + squareSize * move.getSquareY(), squareSize, squareSize };
				SDL_RenderFillRect(renderer, &rect);
			}
		}

		m_game->getBoards()[index].renderBoard(renderer, m_redCross, m_blueCross, boardX, boardY, m_boardRenderSize);
	}

	m_player1Text->renderText(renderer);
	m_player2Text->renderText(renderer);
	m_vsText->renderText(renderer);

	m_boardCountText->renderText(renderer);
	m_boardSizeText->renderText(renderer);
	m_lineSizeText->renderText(renderer);

	m_winnerText->renderText(renderer);
	if (m_winnerDisplayText != nullptr)
		m_winnerDisplayText->renderText(renderer);

	for (unsigned int index = 0; index < m_movesTexts.size(); index++){
		switch (index % 2){
		case 0:
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			break;
		case 1:
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			break;
		}
		const int x = m_playingFieldSize;
		SDL_Rect rect = { x + 2 * PADDING_X + (int)(index / m_amountInColumn) * m_movesWidth,
			m_winnerText->getY() + m_winnerText->getHeight() + 2 * PADDING_Y + (index % m_amountInColumn) * m_movesHeight, m_movesWidth, m_movesHeight };
		SDL_RenderFillRect(renderer, &rect);
		m_movesTexts[index]->renderText(renderer);
	}

}

void GameScreen::input(const SDL_Event & event){
	if (event.type != SDL_KEYDOWN)
		return;

	if (event.key.keysym.sym == SDLK_RIGHT)
		m_forward = true;
	else if (event.key.keysym.sym == SDLK_LEFT)
		m_backward = true;
}

void GameScreen::cleanUp(){

	m_moves.clear();
	m_winner = 0;

	delete m_player1;
	m_player1 = nullptr;

	delete m_player2;
	m_player2 = nullptr;

	for (unsigned int index = 0; index < m_movesTexts.size(); index++){
		delete m_movesTexts[index];
		m_movesTexts[index] = nullptr;
	}
	m_movesTexts.clear();

	delete m_player1Text;
	m_player1Text = nullptr;
	delete m_player2Text;
	m_player2Text = nullptr;

	delete m_boardCountText;
	m_boardCountText = nullptr;
	delete m_boardSizeText;
	m_boardSizeText = nullptr;
	delete m_lineSizeText;
	m_lineSizeText = nullptr;

	delete m_winnerText;
	m_winnerText = nullptr;
	delete m_winnerDisplayText;
	m_winnerDisplayText = nullptr;

	delete m_vsText;
	m_vsText = nullptr;

}
