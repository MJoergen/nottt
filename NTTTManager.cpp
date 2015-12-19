#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "GUI/Texture.h"
#include "GUI/TextField.h"
#include <iostream>
#include <vector>
#include "GUI/Text.h"
#include "GUI/Button.h"

bool init();
void loop();
void close();

const char* TITLE = "No Tic Tac Toe";			//The title of the window
const int WINDOW_WIDTH = 800;					//The width of the window
const int WINDOW_HEIGHT = 600;					//The height of the window
const int FONT_SIZE = 30;						//The size of the font
const char* FONT_PATH = "<insert-path-here>";	//The path to the font
extern const int PADDING_X = 3, PADDING_Y = 5;	//The padding along the axises

SDL_Window* g_window = NULL;			//Pointer pointing to a struct representing the window
SDL_Renderer* g_renderer = NULL;		//Pointer pointing to a struct representing the renderer
TTF_Font* g_font = NULL;				//Pointer pointing to the representation of the font
int g_textHeight = 0;					//The 'maximum' text height used with the font

Text *boardCountText = nullptr, *boardSizeText = nullptr, *lineSizeText = nullptr;						//Text-elements in the GUI
TextField *boardCountTextField = nullptr, *boardSizeTextField = nullptr, *lineSizeTextField = nullptr;	//TextField-elements in the GUI
Button *startGameButton = nullptr;																		//The button to start the game in the GUI

bool isStarted = false; //Boolean used to indicate if the game is started

/*

REMEMBER to insert a valid font path before running the program.
Dependencies: SDL, SDL_image, SDL_ttf

int main(int argc, char *argv[]) {
	if (!init()){ //Initializes SDL, SDL_image, SDL_ttf and global variables. If something went wrong. The program is terminated.
		std::cout << "Press ENTER to continue..." << std::endl;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return 1;
	}

	loop(); 
	close();

	std::cout << "Press ENTER to continue..." << std::endl;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return 0;
}
*/

/**
* Initializes SDL, SDL_image, SDL_ttf and defines global variables.
* @return A boolean representing the whether the everything initialized correctly or not.
*  If true everything is correctly initialized.
*  If false something went wrong in the initialization process.
*/
bool init(){
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){ //Initializes every part of SDL
		std::cout << "Failed initializing SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	int initFlags = IMG_INIT_PNG;
	if (!(IMG_Init(initFlags) & initFlags)){ //Initializes the PNG part of SDL_image
		std::cout << "Failed initializing SDL_image: " << IMG_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	if (TTF_Init() == -1){ //Initializes SDL_ttf
		std::cout << "Failed initializing SDL_ttf: " << TTF_GetError() << std::endl;
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	g_window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_WIDTH, SDL_WINDOW_SHOWN); //Creates a window
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
	
	return true;
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
	isStarted = true;

	//TODO
}

/**
* Runs the main loop and responds to event calls.
*/
void loop(){
	bool quit = false;
	
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

		while (SDL_PollEvent(&event) != 0){ //Get the waiting events, if any
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

		boardCountText->renderText();
		boardSizeText->renderText();
		lineSizeText->renderText();

		boardCountTextField->renderTextField(SDL_GetTicks());
		boardSizeTextField->renderTextField(SDL_GetTicks());
		lineSizeTextField->renderTextField(SDL_GetTicks());

		startGameButton->renderButton();

		SDL_RenderPresent(g_renderer); //Updates the screen

		SDL_Delay(10); //Sleeps for 10 milliseconds
	}
}

/**
* Deletes the used pointers. Quits SDL, SDL_image and SDL_ttf.
*/
void close(){

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