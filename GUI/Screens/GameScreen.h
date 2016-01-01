#ifndef _GAME_SCREEN_H_
#define _GAME_SCREEN_H_

#include "Screen.h"
#include "../Text.h"
#include <vector>
#include "../../NTTTGame.h"
#include "../../NTTTPlayer.h"

//TODO: Finish GameScreen

class GameScreen : public Screen {
public:
	GameScreen(ScreenState* currentState, NTTTGame *game, TTF_Font *movesFont);
	virtual ~GameScreen();
	virtual void init(TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height);
	virtual void render(SDL_Renderer* renderer) const;
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
	void initialize();
	bool m_isInitialized;
	NTTTPlayer *m_player1, *m_player2;
	NTTTGame *m_game;
	unsigned int m_boardCount, m_boardSize, m_lineSize, m_gameSeed;

	TTF_Font *m_headlineFont = nullptr, *m_guiFont = nullptr;
	unsigned int m_width, m_height;

	//Modified copy from GameInfoViewer ----->
	Text *m_player1Text = nullptr, *m_player2Text = nullptr, *m_vsText = nullptr, *m_boardCountText = nullptr, *m_boardSizeText = nullptr, *m_lineSizeText = nullptr;
	Text *m_winnerText = nullptr, *m_winnerDisplayText = nullptr;
	unsigned int m_amountInColumn, m_amountInRow;

	std::vector<Text*> m_movesTexts;

	TTF_Font *m_movesFont = nullptr;
	int m_headlineHeight, m_movesHeight, m_movesWidth;
	unsigned int m_maxMoves, m_excessMoves = 0;

	bool m_failedFontInitHeadline = false, m_failedFontInitMoves = false;
	//<-----

	//Modified copy from NTTTManager ----->
	int manageGame();
	void writeLog(const int winner) const;
	const int BOARD_PADDING = 30;					//The padding between the borders

	int m_playingFieldSize;
	int m_gridSize;
	int m_boardRenderSize;

	std::vector<NTTTMove> m_moves;
	//<-----
};

#endif