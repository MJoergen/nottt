#ifndef _GAME_INFO_VIEWER_H_
#define _GAME_INFO_VIEWER_H_

#include <SDL.h>
#include "Text.h"
#include <vector>
#include "../NTTTMove.h"

class GameInfoViewer {

private:

	Text *m_player1 = nullptr, *m_player2 = nullptr, *m_vs = nullptr, *m_boardCount = nullptr, *m_boardSize = nullptr, *m_lineSize = nullptr;
	Text *m_winnerText = nullptr, *m_winner = nullptr;

	std::vector<Text*> m_moves;

	TTF_Font *m_headlineFont = nullptr, *m_movesFont = nullptr;
	int m_headlineHeight, m_movesHeight, m_movesWidth;

	int m_amountInColumn;

public:
	
	GameInfoViewer();
	virtual ~GameInfoViewer();

	void renderGameInfoViewer() const;

	void init();
	void cleanUp();
	void addMove(NTTTMove move);
	void setWinner(int index); //Set the winner to player1 (index = 1) or player2 (index = 2)

};

#endif