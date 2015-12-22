#include "GameInfoViewer.h"
#include "../NTTTManager.h"

GameInfoViewer::GameInfoViewer(){
}

void GameInfoViewer::init(){
	
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
	
}

GameInfoViewer::~GameInfoViewer(){
	cleanUp();
}

void GameInfoViewer::renderGameInfoViewer() const{

}
