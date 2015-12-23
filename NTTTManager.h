#ifndef _NTTTMANAGER_H_
#define _NTTTMANAGER_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include "GUI/Button.h"
#include "GUI/Text.h"
#include "GUI/TextField.h"
#include "GUI/RadioButton.h"
#include "GUI/Texture.h"
#include "GUI/GameInfoViewer.h"
#include <algorithm>

#include "NTTTGame.h"
#include "NTTTPlayer.h"

class NTTTManager
{
    public:
        bool init();
        void loop();
        void close();
        void onClick();
        int manageGame();
		void writeLog(const int winner) const;
		bool isGameStarted();
		NTTTPlayer* getPlayer1() { return m_player1; }
		NTTTPlayer* getPlayer2() { return m_player2; }
		const int getPlayingFieldSize() { return windowSize; }

    // These variables are accessed from OUTSIDE the NTTTManager class via the
    // global variable g_NtttManager
    // TODO: Put these variables in a separate class
    public:
        SDL_Renderer* g_renderer = NULL;				//Pointer pointing to a struct representing the renderer
        Texture *g_redCross;
        Texture *g_blueCross;
        Texture *g_checkMark;
        const int PADDING_X = 3, PADDING_Y = 5;			//The padding along the axises
        int g_textHeight = 0;							//The 'maximum' text height used with the font
        TTF_Font* g_font = NULL;						//Pointer pointing to the representation of the font
		const char* TITLE = "No Tic Tac Toe";			//The title of the window
		const int WINDOW_WIDTH = 900;					//The width of the window
		const int WINDOW_HEIGHT = 600;					//The height of the window

		TTF_Font *g_headlineFont = nullptr, *g_movesFont = nullptr;
		int g_headlineHeight, g_movesHeight, g_movesWidth;
		bool g_failedFontInitHeadline = false, g_failedFontInitMoves = false;

		Text *g_player1 = nullptr, *g_player2 = nullptr, *g_vs = nullptr, *g_boardCount = nullptr, *g_boardSize = nullptr, *g_lineSize = nullptr;
		Text *g_winnerText = nullptr, *g_winner = nullptr;
		int g_amountInColumn;

		std::vector<Text*> g_moves;

		NTTTGame *g_game;
		const int FONT_SIZE = 30;								//The size of the font
		const std::string FONT_PATH = "Junicode-Regular.ttf";	//The path to the font

    // These variables are only used WITHIN the NTTTManager class
    private:
        const int BOARD_PADDING = 30;					//The padding between the borders
		void cleanUpGameInfoViewer();
		void initGameInfoViewer();

        SDL_Window* g_window = NULL;			//Pointer pointing to a struct representing the window

		Text *boardCountText = nullptr, *boardSizeText = nullptr,
			*lineSizeText = nullptr, *manualModeText = nullptr, *logText = nullptr;										//Text-elements in the GUI
        TextField *boardCountTextField = nullptr, *boardSizeTextField = nullptr, *lineSizeTextField = nullptr;			//TextField-elements in the GUI
        Button *startGameButton = nullptr;																				//The button to start the game in the GUI
		RadioButton *manualModeRadioButton = nullptr, *logRadioButton = nullptr;
		GameInfoViewer *m_gameInfoViewer = nullptr;

        SDL_Thread* gameThread;

        bool isGameThreadRunning = false;
		bool quit = false;
		bool isStarted = false; //Boolean used to indicate if the game is started
		bool backward = false;
		bool forward = false;
		bool initGraphics = false;

		NTTTPlayer *m_player1, *m_player2;

		const int windowSize = std::min(WINDOW_WIDTH, WINDOW_HEIGHT);
        int gridSize;
		int boardRenderSize;

		std::vector<NTTTMove> m_moves;

}; // end of NTTTManager

extern NTTTManager g_NtttManager;

#endif // _NTTTGLOBAL_H_

