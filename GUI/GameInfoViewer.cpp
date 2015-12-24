#include "GameInfoViewer.h"
#include "../NTTTManager.h"

GameInfoViewer::GameInfoViewer(){


	m_headlineFont = TTF_OpenFont((g_NtttManager.FONT_PATH).c_str(), (int)(g_NtttManager.FONT_SIZE * 1.1));
	if (m_headlineFont == NULL){
		std::cout << "Failed loading font: " << TTF_GetError() << std::endl;
		m_headlineFont = g_NtttManager.g_font;
		m_failedFontInitHeadline = true;
	}

	m_movesFont = TTF_OpenFont((g_NtttManager.FONT_PATH).c_str(), (int)(g_NtttManager.FONT_SIZE * 0.5));
	if (m_movesFont == NULL){
		std::cout << "Failed loading font: " << TTF_GetError() << std::endl;
		m_movesFont = g_NtttManager.g_font;
		m_failedFontInitMoves = true;
	}

	m_headlineHeight = TTF_FontHeight(m_headlineFont);
	m_movesHeight = TTF_FontHeight(m_movesFont);

}

void GameInfoViewer::init(){
	cleanUp();
	
	const int x = g_NtttManager.getPlayingFieldSize();
	const int y = 0;
	const int width = g_NtttManager.WINDOW_WIDTH - x;

	m_vs = new Text(" vs ", m_headlineFont, { 0, 0, 0 }, x, y);
	m_vs->set(x - m_vs->getWidth() / 2 + width / 2, g_NtttManager.PADDING_Y);

	m_player1 = new Text(g_NtttManager.getPlayer1()->getName(), m_headlineFont, { 255, 0, 0 }, x, m_vs->getY());
	m_player2 = new Text(g_NtttManager.getPlayer2()->getName(), m_headlineFont, { 0, 0, 255 }, m_vs->getX() + m_vs->getWidth(), m_vs->getY());
	m_player1->setX(m_vs->getX() - m_player1->getWidth());

	m_boardCount = new Text("BoardCount: " + std::to_string(g_NtttManager.g_game->getBoardCount()), x, m_vs->getY() + m_vs->getHeight() + g_NtttManager.PADDING_Y * 2);
	m_boardCount->setX(x + width / 2 - m_boardCount->getWidth() / 2);

	m_boardSize = new Text("BoardSize: " + std::to_string(g_NtttManager.g_game->getBoardSize()), x, m_boardCount->getY() + m_boardCount->getHeight() + g_NtttManager.PADDING_Y);
	m_boardSize->setX(x + width / 2 - m_boardSize->getWidth() / 2);

	m_lineSize = new Text("LineSize: " + std::to_string(g_NtttManager.g_game->getLineSize()), x, m_boardSize->getY() + m_boardSize->getHeight() + g_NtttManager.PADDING_Y);
	m_lineSize->setX(x + width / 2 - m_lineSize->getWidth() / 2);

	m_winnerText = new Text("Winner: ", x + g_NtttManager.PADDING_X, m_lineSize->getY() + m_lineSize->getHeight() + g_NtttManager.PADDING_Y);

	int w, h;

	TTF_SizeText(m_movesFont, (std::to_string(g_NtttManager.g_game->getBoardCount()) + "  : (8,8)").c_str(), &w, &h);

	m_movesWidth = w + g_NtttManager.PADDING_X * 2; //Estimate

	m_amountInColumn = (int)((g_NtttManager.WINDOW_HEIGHT - (m_winnerText->getY() + m_winnerText->getHeight() + 3 * g_NtttManager.PADDING_Y)) / m_movesHeight);
}

void GameInfoViewer::addMove(NTTTMove move){
	/*
	const int x = g_NtttManager.getPlayingFieldSize();

	m_moves.push_back(new Text(std::to_string(move.getBoardNumber()) + " : (" + std::to_string(move.getSquareX())
	+ ", " + std::to_string(move.getSquareY()) + ")", m_movesFont , { 0, 255, 0 }, x + g_NtttManager.PADDING_X + (int)(m_moves.size() / m_amountInColumn) * m_movesWidth,
	m_winnerText->getY() + m_winnerText->getHeight() + 2 * g_NtttManager.PADDING_Y + (m_moves.size() % m_amountInColumn) * m_movesHeight));

	*/
}

void GameInfoViewer::setWinner(int index){	//Set the winner to player1 (index = 1) or player2 (index = 2)
	/*
	const int x = g_NtttManager.getPlayingFieldSize();
	const int width = g_NtttManager.WINDOW_WIDTH - x;
	switch (index){
	case 1:
	m_winner = new Text(g_NtttManager.getPlayer1()->getName(), {255, 0, 0}, x, m_lineSize->getY() + m_lineSize->getHeight() + g_NtttManager.PADDING_Y);
	m_winner->setX(x + width - g_NtttManager.PADDING_X - m_winner->getWidth());
	break;
	case 2:
	m_winner = new Text(g_NtttManager.getPlayer2()->getName(), {0, 0, 255}, x, m_lineSize->getY() + m_lineSize->getHeight() + g_NtttManager.PADDING_Y);
	m_winner->setX(x + width - g_NtttManager.PADDING_X - m_winner->getWidth());
	break;
	}
	*/
}

void GameInfoViewer::cleanUp() {
	delete m_player1;
	m_player1 = nullptr;
	delete m_player2;
	m_player2 = nullptr;

	delete m_boardCount;
	m_boardCount = nullptr;
	delete m_boardSize;
	m_boardSize = nullptr;
	delete m_lineSize;
	m_lineSize = nullptr;

	delete m_winnerText;
	m_winnerText = nullptr;
	delete m_winner;
	m_winner = nullptr;

	delete m_vs;
	m_vs = nullptr;
}

GameInfoViewer::~GameInfoViewer(){

	if (!m_failedFontInitHeadline)
		TTF_CloseFont(m_headlineFont);
	if (!m_failedFontInitMoves)
		TTF_CloseFont(m_movesFont);

	cleanUp();
}

void GameInfoViewer::renderGameInfoViewer() const{

	if (m_player1 == nullptr)
		return;

	m_player1->renderText();
	m_player2->renderText();
	m_vs->renderText();

	m_boardCount->renderText();
	m_boardSize->renderText();
	m_lineSize->renderText();

	m_winnerText->renderText();
	if (m_winner != nullptr)
		m_winner->renderText();
}
