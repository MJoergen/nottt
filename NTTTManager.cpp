#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_thread.h>
#include "GUI/Texture.h"
#include "GUI/TextField.h"
#include <iostream>
#include <vector>
#include "GUI/Text.h"
#include "GUI/Button.h"
#include "NTTTGame.h"
#include "NTTTPlayerIce.h"
#include "NTTTPlayerMike.h"
#include <algorithm>

bool init();
void loop();
void close();

const char* TITLE = "No Tic Tac Toe";			//The title of the window
const int WINDOW_WIDTH = 800;					//The width of the window
const int WINDOW_HEIGHT = 600;					//The height of the window
const int FONT_SIZE = 30;						//The size of the font
const char* FONT_PATH = "Junicode-Regular.ttf";	//The path to the font
extern const int PADDING_X = 3, PADDING_Y = 5;	//The padding along the axises
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

/**
* Initializes SDL, SDL_image, SDL_ttf and defines global variables.
* @return A boolean representing the whether the everything initialized correctly or not.
*  If true everything is correctly initialized.
*  If false something went wrong in the initialization process.
*/
bool init(){
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){ //Initializes every part of SDL2
		std::cout << "Failed initializing SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	int initFlags = IMG_INIT_PNG;
	if (!(IMG_Init(initFlags) & initFlags)){ //Initializes the PNG part of SDL2_image
		std::cout << "Failed initializing SDL_image: " << IMG_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	if (TTF_Init() == -1){ //Initializes SDL2_ttf
		std::cout << "Failed initializing SDL_ttf: " << TTF_GetError() << std::endl;
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	g_window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN); //Creates a window
	if (g_window == NULL){
		std::cout << "Failed creating window: " << SDL_GetError() << std::endl;
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED); //Creates the renderer used to render to the window
	if (g_renderer == NULL){
		std::cout << "Failed creating renderer: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(g_window);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	g_font = TTF_OpenFont(FONT_PATH, FONT_SIZE); //Creates the font used in the application
	if (g_font == NULL){
		std::cout << "Failed loading font: " << TTF_GetError() << std::endl;
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return false;
	}
	Texture heightMeasureTexture("|,.-gWjIi", { 0, 0, 0 });

	g_textHeight = heightMeasureTexture.getHeight(); //Measures the height of the texture to create an estimate on the max height (from the lowest point to the highest) of the font

	g_redCross = new Texture("RedCross.png");
	g_blueCross = new Texture("BlueCross.png");
	g_game = new NTTTGame();
	
	return true;
}

int manageGame( void* data){

	//This is where the game goes.

	//Some test code

	NTTTPlayerIce ice;
	NTTTPlayerMike mike;

	NTTTPlayer* player1;
	NTTTPlayer* player2;

	NTTTPlayer::OrderChoice iceOrderChoice, mikeOrderChoice;
	iceOrderChoice = ice.chooseOrder(*g_game);
	mikeOrderChoice = mike.chooseOrder(*g_game);

	if (iceOrderChoice == mikeOrderChoice){
		if (rand() % 2 == 0){ //TODO: Make this random by srand() or other method.
			std::cout << "Ice starts" << std::endl;
			player1 = &ice;
			player2 = &mike;
		}
		else {
			std::cout << "Mike starts" << std::endl;
			player1 = &mike;
			player2 = &ice;
		}
	}
	else if (iceOrderChoice == NTTTPlayer::FIRST || mikeOrderChoice == NTTTPlayer::LAST){
		std::cout << "Ice starts" << std::endl;
		player1 = &ice;
		player2 = &mike;
	}
	else{
		std::cout << "Mike starts" << std::endl;
		player1 = &mike;
		player2 = &ice;
	}


	int player = 1;
	NTTTMove move(0, 0, 0);
	for (int i = 0; i<10 && !quit; ++i)
	{
		switch (player)
		{
		case 1:
			move = player1->performMove(*g_game);
			g_game->makeMove(move, NTTTBoard::RED);
			break;
		case 2:
			move = player2->performMove(*g_game);
			g_game->makeMove(move, NTTTBoard::BLUE);
			break;
		}
		player = 3 - player;
		SDL_Delay(500);
	}
	
	//isGameThreadRunning must be set to false just before the end.
	isGameThreadRunning = false;

	return 0;
}

void onClick(){ //Function called when the start game button is pressed
	if (isStarted){ //Returns if the game already is started
		std::cout << "Failed to start game: A game is already in progress." << std::endl;
		return;
	}
	const unsigned int boardCount = std::stoi(boardCountTextField->getContent());
	const unsigned int boardSize = std::stoi(boardSizeTextField->getContent());
	const unsigned int lineSize = std::stoi(lineSizeTextField->getContent());
	std::cout << "Starts the game with the following settings: { BoardCount: "
		<< boardCount << ", BoardSize: " << boardSize << ", LineSize: " << lineSize << " }" << std::endl;

	g_game->NewGame(boardCount, boardSize, lineSize); // Prepares the game with the chosen settings
	isGameThreadRunning = true;
	gameThread = SDL_CreateThread(manageGame, "Game Thread", NULL);
	if (gameThread == NULL){
		std::cout << "Failed to create thread: " << SDL_GetError() << std::endl;
		isGameThreadRunning = false;
	} else
		isStarted = true;

	gridSize = (int)ceil(sqrt(g_game->getBoardCount()));
	boardRenderSize = (windowSize - (1 + gridSize) * BOARD_PADDING) / gridSize;
}

/**
* Runs the main loop and responds to event calls.
*/
void loop(){
	
	boardCountText = new Text("BoardCount: ", PADDING_X, PADDING_Y);
	boardCountTextField = new TextField(TextField::NUMBER, "3", PADDING_X + boardCountText->getWidth() + boardCountText->getX(), PADDING_Y, 60, -1);

	boardSizeText = new Text("BoardSize: ", PADDING_X + boardCountTextField->getWidth() + boardCountTextField->getX(), PADDING_Y);
	boardSizeTextField = new TextField(TextField::NUMBER, "4", PADDING_X + boardSizeText->getWidth() + boardSizeText->getX(), PADDING_Y, 30, 8);

	lineSizeText = new Text("LineSize: ", PADDING_X + boardSizeTextField->getWidth() + boardSizeTextField->getX(), PADDING_Y);
	lineSizeTextField = new TextField(TextField::NUMBER, "3", PADDING_X + lineSizeText->getWidth() + lineSizeText->getX(), PADDING_Y, 30, 8);

	startGameButton = new Button("Start Game", PADDING_X + lineSizeTextField->getWidth() + lineSizeTextField->getX(), PADDING_Y);

	startGameButton->registerClickFunc(onClick);

	while (!quit){ //Runs until the program quits

		SDL_Event event;

		while (SDL_PollEvent(&event) != 0){ //Get the pending events, if any
			if (event.type == SDL_QUIT){ //Quits the program, if a SDL_QUIT event has been triggered (e.g. when the window is closed)
				quit = true;
			}
			else if (event.type == SDL_TEXTINPUT){ //Responds to textinput
				if (boardCountTextField->isSelected())
					boardCountTextField->onKeyPress(event.key.keysym, event.text.text);
				else if (boardSizeTextField->isSelected())
					boardSizeTextField->onKeyPress(event.key.keysym, event.text.text);
				else if (lineSizeTextField->isSelected())
					lineSizeTextField->onKeyPress(event.key.keysym, event.text.text);
			}
			else if (event.type == SDL_KEYDOWN){ //Responds to keyboardinput
				if (boardCountTextField->isSelected())
					boardCountTextField->onKeyPress(event.key.keysym, "");
				else if (boardSizeTextField->isSelected())
					boardSizeTextField->onKeyPress(event.key.keysym, "");
				else if (lineSizeTextField->isSelected())
					lineSizeTextField->onKeyPress(event.key.keysym, "");
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN){ //Responds to mouseinput
				int x, y;
				SDL_GetMouseState(&x, &y);

				if (boardCountTextField->isSelected())
					boardCountTextField->deselect();
				if (boardSizeTextField->isSelected())
					boardSizeTextField->deselect();
				if (lineSizeTextField->isSelected())
					lineSizeTextField->deselect();

				if (boardCountTextField->isInside(x, y))
					boardCountTextField->select();
				else if (boardSizeTextField->isInside(x, y))
					boardSizeTextField->select();
				else if (lineSizeTextField->isInside(x, y))
					lineSizeTextField->select();
				else if (startGameButton->isInside(x, y))
					startGameButton->click();
			}
		}

		SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
		SDL_RenderClear(g_renderer); //Clears the screen

		if (isStarted){

			if (!isGameThreadRunning){
				int status;

				SDL_WaitThread(gameThread, &status);
				isStarted = false;
				std::cout << "Game ended: " << status << std::endl;
			}
			
			for (int index = 0; index < g_game->getBoardCount(); index++){
				g_game->getBoards()[index].renderBoard(BOARD_PADDING * (index % gridSize + 1) + boardRenderSize * (index % gridSize), BOARD_PADDING * (int)(index / gridSize + 1) + boardRenderSize * (int)(index / gridSize), boardRenderSize);
			}
		}
		else {
			boardCountText->renderText();
			boardSizeText->renderText();
			lineSizeText->renderText();

			boardCountTextField->renderTextField(SDL_GetTicks());
			boardSizeTextField->renderTextField(SDL_GetTicks());
			lineSizeTextField->renderTextField(SDL_GetTicks());

			startGameButton->renderButton();
		}

		SDL_RenderPresent(g_renderer); //Updates the screen

		SDL_Delay(50); //Sleeps for 10 milliseconds
	}

	if (isGameThreadRunning){ // Wait for the game thread to end and clean it up.
		int status;

		SDL_WaitThread(gameThread, &status);
		isStarted = false;
		std::cout << "Game ended: " << status << std::endl;
	}
}

/**
* Deletes the used pointers. Quits SDL2, SDL2_image and SDL2_ttf.
*/
void close(){
	
	delete g_game;
	g_game = nullptr;

	delete g_redCross;
	g_redCross = nullptr;
	delete g_blueCross;
	g_blueCross = nullptr;

	delete boardCountText;
	boardCountText = nullptr;
	delete boardCountTextField;
	boardCountTextField = nullptr;

	delete boardSizeText;
	boardSizeText = nullptr;
	delete boardSizeTextField;
	boardSizeTextField = nullptr;

	delete lineSizeText;
	lineSizeText = nullptr;
	delete lineSizeTextField;
	lineSizeTextField = nullptr;

	delete startGameButton;
	startGameButton = nullptr;

	TTF_CloseFont(g_font);

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	g_renderer = nullptr;
	g_window = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool is_number(const std::string& s) //Checks if a string is a number
{
	return !s.empty() && s.find_first_not_of("0123456789") == std::string::npos;
}
