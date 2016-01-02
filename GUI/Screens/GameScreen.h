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
	struct GameData{
		ScreenState* currentState;
		NTTTGame *game;
		TTF_Font *movesFont;
		int *boardCount;
		int *boardSize;
		int *lineSize;
		int *gameSeed;
		bool *manualMode;
		bool *writeLog;
		std::string *logName;
		bool *quit;
	};
	GameScreen(GameData data);
	virtual ~GameScreen();
	virtual void init(SDL_Renderer *renderer, TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height);
	virtual void render(SDL_Renderer* renderer) const;
	virtual void input(const SDL_Event & e);
	virtual void update(SDL_Renderer *renderer);
	virtual void prepareForQuit();
	virtual void cleanUp();
	int manageGame();

	//Modified copy from GameInfoViewer ----->
	unsigned int getMovesCount(){
		if (m_movesTexts.size() < m_maxMoves)
			return m_movesTexts.size();
		else
			return m_maxMoves + m_excessMoves;
	}
	//<-----

private:
	bool m_isThreadProperlyClosed = true;

	void initialize(SDL_Renderer *renderer);
	bool m_isInitialized;
	NTTTPlayer *m_player1, *m_player2;
	NTTTGame *m_game;
	unsigned int m_winner = 0;

	int *m_boardCount = nullptr, *m_boardSize = nullptr, *m_lineSize = nullptr, *m_gameSeed = nullptr;
	bool *m_manualMode = nullptr, *m_writeLog = nullptr;
	std::string *m_logName = nullptr;
	
	bool m_forward = false, m_backward = false;

	bool m_isGameThreadRunning = false;
	SDL_Thread *m_gameThread = nullptr;

	TTF_Font *m_headlineFont = nullptr, *m_guiFont = nullptr;
	unsigned int m_width, m_height;
	bool *m_quit = nullptr;

	//Modified copy from GameInfoViewer ----->
	Text *m_player1Text = nullptr, *m_player2Text = nullptr, *m_vsText = nullptr, *m_boardCountText = nullptr, *m_boardSizeText = nullptr, *m_lineSizeText = nullptr;
	Text *m_winnerText = nullptr, *m_winnerDisplayText = nullptr;
	unsigned int m_amountInColumn, m_amountInRow;

	std::vector<Text*> m_movesTexts;

	TTF_Font *m_movesFont = nullptr;
	int m_headlineHeight, m_movesHeight, m_movesWidth;
	unsigned int m_maxMoves, m_excessMoves = 0;
	//<-----

	//Modified copy from NTTTManager ----->
	void writeLog(const int winner) const;
	const int BOARD_PADDING = 30;					//The padding between the borders

	int m_playingFieldSize;
	int m_gridSize;
	int m_boardRenderSize;

	std::vector<NTTTMove> m_moves;
	//<-----

	void initializeWinnerDisplayText(SDL_Renderer *renderer);
	void addMove(SDL_Renderer *renderer, NTTTMove move);
	void removeMove();
};

#endif