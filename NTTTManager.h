#ifndef _NTTTMANAGER_H_
#define _NTTTMANAGER_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include "GUI/Button.h"
#include "GUI/Text.h"
#include "GUI/TextField.h"
#include "GUI/RadioButton.h"
#include "GUI/Texture.h"
#include <algorithm>

#include "NTTTGame.h"
#include "NTTTPlayer.h"

#include "GUI/Screens/Screen.h"

class NTTTManager
{
	public:
        bool init();
        void loop();
		void close();
		void onReadLogClick();
		void writeLog(const int winner) const;

    // These variables are accessed from OUTSIDE the NTTTManager class via the
    // global variable g_NtttManager
    // TODO: Put these variables in a separate class
	public:
        const int PADDING_X = 3, PADDING_Y = 5;			//The padding along the axises
        int g_textHeight = 0;							//The 'maximum' text height used with the font
				
    // These variables are only used WITHIN the NTTTManager class
	private:
		Texture *m_redCross;
		Texture *m_blueCross;
		Texture *m_checkMark;

		TTF_Font* m_font = NULL;						//Pointer pointing to the representation of the font
		const int FONT_SIZE = 30;								//The size of the font
		const std::string FONT_PATH = "Junicode-Regular.ttf";	//The path to the font

		NTTTGame *g_game;

		const char* TITLE = "No Tic Tac Toe";			//The title of the window
		const int WINDOW_WIDTH = 900;					//The width of the window
		const int WINDOW_HEIGHT = 600;					//The height of the window

		SDL_Renderer* m_renderer = NULL;				//Pointer pointing to a struct representing the renderer
		
		int m_boardCount = 3, m_boardSize = 4, m_lineSize = 3, m_gameSeed = 12345;
		bool m_manualMode = false, m_writeLog = false;
		std::string m_logName = "log.txt";
		
		Screen::ScreenState m_currentState = Screen::MAIN_MENU;
		Screen *m_botsScreen = nullptr, *m_mainMenuScreen = nullptr, *m_newGameScreen = nullptr, *m_gameScreen = nullptr;

		TTF_Font *m_movesFont = nullptr;

		void render() const;
		void input(SDL_Event & e);
		void update();

        SDL_Window* g_window = NULL;			//Pointer pointing to a struct representing the window

        bool quit = false;

}; // end of NTTTManager

extern NTTTManager g_NtttManager;

#endif // _NTTTGLOBAL_H_

