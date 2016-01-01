#include "GameScreen.h"
#include <string>
#include <algorithm>


int GameScreen::manageGame(){
	m_gridSize = (int)ceil(sqrt(m_boardCount));
	m_boardRenderSize = (m_playingFieldSize - (1 + m_gridSize) * BOARD_PADDING) / m_gridSize;
	(*m_currentState) = GAME;
	return 0;
}

GameScreen::GameScreen(ScreenState* currentState, NTTTGame * game, TTF_Font *movesFont){
	m_currentState = currentState;
	m_game = game;
	m_movesFont = movesFont;
}

GameScreen::~GameScreen(){
	cleanUp();
}

void GameScreen::úpdate() {
	if (!m_isInitialized){
		initialize();
		m_isInitialized = true;
	}
}

void GameScreen::initialize(){
	cleanUp();

	const int x = m_playingFieldSize;
	const int y = 0;
	const int width = m_width - x;

	m_vsText = new Text(" vs ", m_headlineFont, { 0, 0, 0 }, x, y);
	m_vsText->set(x - m_vsText->getWidth() / 2 + width / 2, PADDING_Y);

	m_player1Text = new Text(m_player1->getName(), m_headlineFont, { 255, 0, 0 }, x, m_vsText->getY());
	m_player2Text = new Text(m_player1->getName(), m_headlineFont, { 0, 0, 255 }, m_vsText->getX() + m_vsText->getWidth(), m_vsText->getY());
	m_player1Text->setX(m_vsText->getX() - m_player1Text->getWidth());

	m_boardCountText = new Text("BoardCount: " + std::to_string(m_game->getBoardCount()), x, m_vsText->getY() + m_vsText->getHeight() + PADDING_Y * 2);
	m_boardCountText->setX(x + width / 2 - m_boardCountText->getWidth() / 2);

	m_boardSizeText = new Text("BoardSize: " + std::to_string(m_game->getBoardSize()), x, m_boardCountText->getY() + m_boardCountText->getHeight() + PADDING_Y);
	m_boardSizeText->setX(x + width / 2 - m_boardSizeText->getWidth() / 2);

	m_lineSizeText = new Text("LineSize: " + std::to_string(m_game->getLineSize()), x, m_boardSizeText->getY() + m_boardSizeText->getHeight() + PADDING_Y);
	m_lineSizeText->setX(x + width / 2 - m_lineSizeText->getWidth() / 2);

	m_winnerText = new Text("Winner: ", x + PADDING_X, m_lineSizeText->getY() + m_lineSizeText->getHeight() + PADDING_Y);

	int w, h;

	TTF_SizeText(m_movesFont, (std::to_string(m_boardCount) + "  : (8,8)").c_str(), &w, &h);

	m_movesWidth = w + PADDING_X * 2; //Estimate

	m_amountInColumn = (int)((m_height - (m_winnerText->getY() + m_winnerText->getHeight() + 3 * PADDING_Y)) / m_movesHeight);
	m_amountInRow = (int)((width - PADDING_X * 2) / m_movesWidth);
	m_maxMoves = m_amountInColumn * m_amountInRow;

}

void GameScreen::init(TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height){
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

	for (unsigned int index = 0; index < m_boardCount; index++){

		int boardX = BOARD_PADDING * (index % m_gridSize + 1) + m_boardRenderSize * (index % m_gridSize);
		int boardY = BOARD_PADDING * (int)(index / m_gridSize + 1) + m_boardRenderSize * (int)(index / m_gridSize);
		int squareSize = m_boardRenderSize / m_boardSize;

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

	for (unsigned int index = 0; index < m_moves.size(); index++){
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
}

void GameScreen::cleanUp(){
	
	for (unsigned int index = 0; index < m_moves.size(); index++){
		delete m_movesTexts[index];
		m_movesTexts[index] = nullptr;
	}
	m_movesTexts.resize(0);

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