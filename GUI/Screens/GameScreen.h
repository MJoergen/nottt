#ifndef _GAME_SCREEN_H_
#define _GAME_SCREEN_H_

#include "Screen.h"
#include "../Text.h"
#include <vector>

//TODO: Finish GameScreen

class GameScreen : public Screen {
public:
	virtual ~GameScreen();
	virtual void init(ScreenState* currentState, TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height);
	virtual void render(const SDL_Renderer* renderer) const;
	virtual void input(const SDL_Event & e);
	virtual void úpdate();
	virtual void cleanUp();

	//Copy from GameInfoViewer ----->
	unsigned int getMovesCount(){
		if (m_moves.size() < m_maxMoves)
			return m_moves.size();
		else
			return m_maxMoves + m_excessMoves;
	}
	//<-----

private:
	//Copy from GameInfoViewer ----->
	Text *m_player1 = nullptr, *m_player2 = nullptr, *m_vs = nullptr, *m_boardCount = nullptr, *m_boardSize = nullptr, *m_lineSize = nullptr;
	Text *m_winnerText = nullptr, *m_winner = nullptr;
	int m_amountInColumn, m_amountInRow;

	std::vector<Text*> m_moves;

	TTF_Font *m_headlineFont = nullptr, *m_movesFont = nullptr;
	int m_headlineHeight, m_movesHeight, m_movesWidth;
	unsigned int m_maxMoves, m_excessMoves = 0;

	bool m_failedFontInitHeadline = false, m_failedFontInitMoves = false;
	//<-----
};

#endif