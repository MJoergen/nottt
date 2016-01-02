#include "GameScreen.h"
#include "../../NTTTPlayerIce.h"
#include "../../NTTTPlayerMike.h"
#include <string>
#include <algorithm>

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

	/*
	if (m_logRadioButton->isChecked())
	writeLog(m_winner);
	*/

	//m_isGameThreadRunning must be set to false just before the end.
	m_isGameThreadRunning = false;

	return player;
}

GameScreen::GameScreen(ScreenState* currentState, NTTTGame *game, TTF_Font *movesFont,
	int *boardCount, int *boardSize, int *lineSize, int *gameSeed, bool *manualMode, bool *writeLog, std::string *logName, bool *quit){
	m_currentState = currentState;

	m_game = game;
	m_movesFont = movesFont;

	m_boardCount = boardCount;
	m_boardSize = boardSize;
	m_lineSize = lineSize;
	m_gameSeed = gameSeed;
	m_manualMode = manualMode;
	m_writeLog = writeLog;
	m_logName = logName;

	m_quit = quit;
}

GameScreen::~GameScreen(){
	cleanUp();
}

void GameScreen::update() {
	if (!m_isInitialized){
		initialize();
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
		initializeWinnerDisplayText();
	}
	for (unsigned int index = getMovesCount(); index < m_moves.size(); index++){
		addMove(m_moves[index]);
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

void GameScreen::addMove(NTTTMove move){
	if (m_movesTexts.size() >= m_maxMoves){
		m_excessMoves++;
		return;
	}
	const int x = m_playingFieldSize;

	m_movesTexts.push_back(new Text(std::to_string(move.getBoardNumber()) + " : (" + std::to_string(move.getSquareX())
		+ ", " + std::to_string(move.getSquareY()) + ")", m_movesFont, { 255, 255, 255 }, x + 2 * PADDING_X + (int)(m_movesTexts.size() / m_amountInColumn) * m_movesWidth,
		m_winnerText->getY() + m_winnerText->getHeight() + 2 * PADDING_Y + (m_movesTexts.size() % m_amountInColumn) * m_movesHeight));

}

void GameScreen::initializeWinnerDisplayText(){
	const int x = m_playingFieldSize;
	const int width = m_width - x;
	switch (m_winner){
	case 1:
		m_winnerDisplayText = new Text(m_player1->getName(), { 255, 0, 0 }, x, m_lineSizeText->getY() + m_lineSizeText->getHeight() + PADDING_Y);
		m_winnerDisplayText->setX(x + width - PADDING_X - m_winnerDisplayText->getWidth());
		break;
	case 2:
		m_winnerDisplayText = new Text(m_player2->getName(), { 0, 0, 255 }, x, m_lineSizeText->getY() + m_lineSizeText->getHeight() + PADDING_Y);
		m_winnerDisplayText->setX(x + width - PADDING_X - m_winnerDisplayText->getWidth());
		break;
	}
}

void GameScreen::initialize(){
	cleanUp();

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

	m_vsText = new Text(" vs ", m_headlineFont, { 0, 0, 0 }, x, y);
	m_vsText->set(x - m_vsText->getWidth() / 2 + width / 2, PADDING_Y);

	m_player1Text = new Text(m_player1->getName(), m_headlineFont, { 255, 0, 0 }, x, m_vsText->getY());
	m_player2Text = new Text(m_player2->getName(), m_headlineFont, { 0, 0, 255 }, m_vsText->getX() + m_vsText->getWidth(), m_vsText->getY());
	m_player1Text->setX(m_vsText->getX() - m_player1Text->getWidth());

	m_boardCountText = new Text("BoardCount: " + std::to_string(*m_boardCount), x, m_vsText->getY() + m_vsText->getHeight() + PADDING_Y * 2);
	m_boardCountText->setX(x + width / 2 - m_boardCountText->getWidth() / 2);

	m_boardSizeText = new Text("BoardSize: " + std::to_string(*m_boardSize), x, m_boardCountText->getY() + m_boardCountText->getHeight() + PADDING_Y);
	m_boardSizeText->setX(x + width / 2 - m_boardSizeText->getWidth() / 2);

	m_lineSizeText = new Text("LineSize: " + std::to_string(*m_lineSize), x, m_boardSizeText->getY() + m_boardSizeText->getHeight() + PADDING_Y);
	m_lineSizeText->setX(x + width / 2 - m_lineSizeText->getWidth() / 2);

	m_winnerText = new Text("Winner: ", x + PADDING_X, m_lineSizeText->getY() + m_lineSizeText->getHeight() + PADDING_Y);

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

void GameScreen::init(TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height){
	//This function is almost empty since the gui depends on the settings (the settings has not yet been chosen)
	m_playingFieldSize = std::min(width, height);
	m_headlineFont = headlineFont;
	m_guiFont = guiFont;
	m_width = width;
	m_height = height;

	Texture texture("0", { 0, 0, 0, 255 });

	m_movesHeight = texture.getHeight();
}

void GameScreen::render(SDL_Renderer* renderer) const{
	
	if (!m_isInitialized)
		return;

	for (unsigned int index = 0; index < (unsigned int) *m_boardCount; index++){

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

		m_game->getBoards()[index].renderBoard(boardX, boardY, m_boardRenderSize);
	}

	if (m_player1Text == nullptr)
		return;

	m_player1Text->renderText();
	m_player2Text->renderText();
	m_vsText->renderText();

	m_boardCountText->renderText();
	m_boardSizeText->renderText();
	m_lineSizeText->renderText();

	m_winnerText->renderText();
	if (m_winnerDisplayText != nullptr)
		m_winnerDisplayText->renderText();

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
		m_movesTexts[index]->renderText();
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