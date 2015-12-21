#ifndef _NTTTMANAGER_H_
#define _NTTTMANAGER_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include "GUI/Button.h"
#include "GUI/Text.h"
#include "GUI/TextField.h"
#include "GUI/Texture.h"

#include "NTTTGame.h"

#if 0
#include "NTTTPlayerIce.h"
#include "NTTTPlayerMike.h"
#endif

class NTTTManager
{
    public:
        bool init();
        void loop();
        void close();
        void onClick();
        int manageGame();


        const char* TITLE = "No Tic Tac Toe";			//The title of the window
        const int WINDOW_WIDTH = 800;					//The width of the window
        const int WINDOW_HEIGHT = 600;					//The height of the window
        const int FONT_SIZE = 30;						//The size of the font
        const char* FONT_PATH = "Junicode-Regular.ttf";	//The path to the font
        const int PADDING_X = 3, PADDING_Y = 5;	//The padding along the axises
        const int BOARD_PADDING = 10;

        SDL_Window* g_window = NULL;			//Pointer pointing to a struct representing the window
        SDL_Renderer* g_renderer = NULL;		//Pointer pointing to a struct representing the renderer
        TTF_Font* g_font = NULL;				//Pointer pointing to the representation of the font
        int g_textHeight = 0;					//The 'maximum' text height used with the font

        Texture *g_redCross, *g_blueCross;
        NTTTGame *g_game;

        Text *boardCountText = nullptr, *boardSizeText = nullptr, *lineSizeText = nullptr;						//Text-elements in the GUI
        TextField *boardCountTextField = nullptr, *boardSizeTextField = nullptr, *lineSizeTextField = nullptr;	//TextField-elements in the GUI
        Button *startGameButton = nullptr;																		//The button to start the game in the GUI

        SDL_Thread* gameThread;

        bool isGameThreadRunning = false;
        bool isStarted = false; //Boolean used to indicate if the game is started
        bool quit = false;


        const int windowSize = std::min(WINDOW_WIDTH, WINDOW_HEIGHT);
        int gridSize;
        int boardRenderSize;

}; // end of NTTTManager

extern NTTTManager g_NtttManager;

#endif // _NTTTGLOBAL_H_

