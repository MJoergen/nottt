#include "NTTTGame.h"
#include "NTTTPlayerIce.h"
#include "NTTTPlayerMike.h"
#include "NTTTManager.h"
#include <fstream>
#include <climits>
#include "GUI/Screens/BotsScreen.h"
#include "GUI/Screens/MainMenuScreen.h"
#include "GUI/Screens/NewGameScreen.h"
#include "GUI/Screens/GameScreen.h"

/**git
* Initializes SDL2, SDL2_image, SDL2_ttf and defines static variables.
* @return A boolean representing the whether the everything initialized correctly or not.
*  If true everything is correctly initialized.
*  If false something went wrong in the initialization process.
*/
bool NTTTManager::init()
{
	
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

	m_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED); //Creates the renderer used to render to the window
	if (m_renderer == NULL){
		std::cout << "Failed creating renderer: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(g_window);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	m_font = TTF_OpenFont(FONT_PATH.c_str(), FONT_SIZE); //Creates the font used in the application
	if (m_font == NULL){
		std::cout << "Failed loading font: " << TTF_GetError() << std::endl;
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(g_window);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	m_movesFont = TTF_OpenFont(FONT_PATH.c_str(), (int)(FONT_SIZE * 0.5));
	if (m_movesFont == NULL){
		std::cout << "Failed loading font: " << TTF_GetError() << std::endl;
		TTF_CloseFont(m_font);
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(g_window);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	m_redCross = new Texture(m_renderer, "RedCross.png");
	m_blueCross = new Texture(m_renderer, "BlueCross.png");
	m_checkMark = new Texture(m_renderer, "CheckMark.png");
	g_game = new NTTTGame();

	//New GUI system

	m_botsScreen = new BotsScreen(&m_currentState);
	m_botsScreen->init(m_renderer, m_font, m_font, WINDOW_WIDTH, WINDOW_HEIGHT);

	m_mainMenuScreen = new MainMenuScreen(&m_currentState);
	m_mainMenuScreen->init(m_renderer, m_font, m_font, WINDOW_WIDTH, WINDOW_HEIGHT);

	NewGameScreen::NewGameData newGameData = { &m_currentState, &m_boardCount, &m_boardSize, &m_lineSize, &m_gameSeed, &m_manualMode, &m_writeLog, &m_logName, m_checkMark };

	m_newGameScreen = new NewGameScreen(newGameData);
	m_newGameScreen->init(m_renderer, m_font, m_font, WINDOW_WIDTH, WINDOW_HEIGHT);

	GameScreen::GameData gameData = { &m_currentState, g_game, m_movesFont, &m_boardCount, &m_boardSize, &m_lineSize, &m_gameSeed, &m_manualMode, &m_writeLog, &m_logName, &quit, m_redCross, m_blueCross };

	m_gameScreen = new GameScreen(gameData);
	m_gameScreen->init(m_renderer, m_font, m_font, WINDOW_WIDTH, WINDOW_HEIGHT);

	return true;
}

/*
void NTTTManager::writeLog(const int winner) const{
	//TODO

	std::ofstream outputFile;
	outputFile.open("logs/" + m_logFileInputTextField->getContent());

	if (!outputFile.good()){
		std::cout << "Failed to open logs/" << m_logFileInputTextField->getContent() << std::endl;
		outputFile.close();
		return;
	}

	outputFile << "# Players: " << std::endl;
	outputFile << "Player 1: " << m_player1->getName() << std::endl;
	outputFile << "Player 2: " << m_player2->getName() << std::endl;
	outputFile << "# Settings:" << std::endl;
	outputFile << "BoardCount: " << g_game->getBoardCount() << std::endl;
	outputFile << "BoardSize: " << g_game->getBoardSize() << std::endl;
	outputFile << "LineSize: " << g_game->getLineSize() << std::endl;
	outputFile << "Game Seed: " << m_gameSeedTextField->getContent() << std::endl;
	outputFile << "# Moves:" << std::endl;
	for (unsigned int index = 0; index < m_moves.size(); index++){
		outputFile << m_moves[index].getBoardNumber() << " : (" << m_moves[index].getSquareX() << ", " << m_moves[index].getSquareY() << ")" << std::endl;
	}
	outputFile << "# Winner:" << std::endl;
	outputFile << "Winner: " << winner << std::endl;

	outputFile.close();

}

void NTTTManager::onReadLogClick(){
	std::ifstream inputFile;
	inputFile.open("logs/" + m_logFileInputTextField->getContent());

	if (!inputFile.good()){
		std::cout << "Failed to open logs/" << m_logFileInputTextField->getContent() << std::endl;
		inputFile.close();
		return;
	}

	unsigned int boardCount;
	unsigned int boardSize;
	unsigned int lineSize;
	int gameSeed;

	bool players = false;
	bool settings = false;
	bool moves = false;
	bool winner = false;

	std::string player1, player2;

	std::vector<NTTTMove> performedMoves;

	unsigned int winneri = 0;
	
	std::string currentLine;
	char c;
	while ((c = inputFile.get()) != EOF){

		if (c != '\n'){
			currentLine += c;
		} else {

			if (currentLine.compare(0, 9, "# Players") == 0){
				players = true;
				settings = false;
				moves = false;
				winner = false;
			}
			else if (currentLine.compare(0, 10, "# Settings") == 0){
				players = false;
				settings = true;
				moves = false;
				winner = false;
			}
			else if (currentLine.compare(0, 7, "# Moves") == 0){
				players = false;
				settings = false;
				moves = true;
				winner = false;
			}
			else if (currentLine.compare(0, 8, "# Winner") == 0){
				players = false;
				settings = false;
				moves = false;
				winner = true;
			}
			else if (players){
				if (currentLine.size() >= 8 && currentLine.compare(0, 8, "Player 1") == 0){
					player1 = currentLine.substr(currentLine.find(":") + 2, currentLine.size());
				}
				else if (currentLine.size() >= 8 && currentLine.compare(0, 8, "Player 2") == 0){
					player2 = currentLine.substr(currentLine.find(":") + 2, currentLine.size());
				}
			}
			else if (settings){
				if (currentLine.size() >= 10 && currentLine.compare(0, 10, "BoardCount") == 0){
					boardCount = std::stoi(currentLine.substr(currentLine.find(":") + 1, currentLine.size()));
				}
				else if (currentLine.size() >= 9 && currentLine.compare(0, 9, "BoardSize") == 0){
					boardSize = std::stoi(currentLine.substr(currentLine.find(":") + 1, currentLine.size()));
				}
				else if (currentLine.size() >= 8 && currentLine.compare(0, 8, "LineSize") == 0){
					lineSize = std::stoi(currentLine.substr(currentLine.find(":") + 1, currentLine.size()));
				}
				else if (currentLine.size() >= 9 && currentLine.compare(0, 9, "Game Seed") == 0){
					gameSeed = std::stoi(currentLine.substr(currentLine.find(":") + 1, currentLine.size()));
				}
			}
			else if (moves){
				int boardIndex = std::stoi(currentLine.substr(0, currentLine.find(":")));
				int squareX = std::stoi(currentLine.substr(currentLine.find("(") + 1, currentLine.find(",")));
				int squareY = std::stoi(currentLine.substr(currentLine.find(",") + 1, currentLine.find(")")));
				performedMoves.push_back(NTTTMove(boardIndex, squareX, squareY));
			}
			else if (winner){
				if (currentLine.size() >= 6 && currentLine.compare(0, 6, "Winner") == 0){
					winneri = std::stoi(currentLine.substr(currentLine.find(":") + 1, currentLine.size()));
				}
			}
			currentLine = "";
		}
	}

	std::cout << "BoardCount: " << boardCount << ", BoardSize: " << boardSize << ", LineSize: " << lineSize << ", Game Seed: " << gameSeed << std::endl;
	std::cout << "Moves: " << performedMoves.size() << std::endl;

	NTTTPlayerIce ice;
	NTTTPlayerMike mike;
	
	if (( player1.compare(ice.getName()) != 0 && player1.compare(mike.getName()) ) || ( player2.compare(ice.getName()) != 0 && player2.compare(mike.getName()) != 0 )){
		std::cout << "Unable to play game, the bots are non-existent" << std::endl;
		inputFile.close();
		return;
	}

	g_game->NewGame(boardCount, boardSize, lineSize);

	m_moves = performedMoves;

	for (unsigned int index = 0; index < performedMoves.size(); index++){
		if (index % 2 == 0){
			g_game->makeMove(performedMoves[index], NTTTBoard::RED);
		}
		else{
			g_game->makeMove(performedMoves[index], NTTTBoard::BLUE);
		}
	}

	//TODO

	inputFile.close();
}

*/

/**
* Runs the main loop and responds to event calls.
*/
void NTTTManager::loop(){
	
	while (!quit){ //Runs until the program quits

		//Input
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0){ //Get the pending events, if any
			if (event.type == SDL_QUIT){ //Quits the program, if a SDL_QUIT event has been triggered (e.g. when the window is closed)
				quit = true;
			}
			else {
				input(event);
			}
		}

		update();
		render();

		SDL_Delay(50); //Sleeps
	}

	m_mainMenuScreen->prepareForQuit();
	m_newGameScreen->prepareForQuit();
	m_botsScreen->prepareForQuit();
	m_gameScreen->prepareForQuit();

}

void NTTTManager::update(){
	switch (m_currentState){
	case Screen::MAIN_MENU:
		m_mainMenuScreen->update(m_renderer);
		break;
	case Screen::NEW_GAME:
		m_newGameScreen->update(m_renderer);
		break;
	case Screen::LOAD_GAME:
		//TODO
		break;
	case Screen::GAME:
		m_gameScreen->update(m_renderer);
		break;
	case Screen::BOTS:
		m_botsScreen->update(m_renderer);
		break;
	case Screen::STATISTICS:
		//TODO
		break;
	}

}

void NTTTManager::input(SDL_Event & event) {
	switch (m_currentState){
	case Screen::MAIN_MENU:
		m_mainMenuScreen->input(event);
		break;
	case Screen::NEW_GAME:
		m_newGameScreen->input(event);
		break;
	case Screen::LOAD_GAME:
		//TODO
		break;
	case Screen::GAME:
		m_gameScreen->input(event);
	case Screen::BOTS:
		m_botsScreen->input(event);
		break;
	case Screen::STATISTICS:
		//TODO
		break;
	case Screen::EXIT:
		quit = true;
		break;
	}
}

void NTTTManager::render() const{

	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	SDL_RenderClear(m_renderer); //Clears the screen
	switch (m_currentState){
	case Screen::MAIN_MENU:
		m_mainMenuScreen->render(m_renderer);
		break;
	case Screen::NEW_GAME:
		m_newGameScreen->render(m_renderer);
		break;
	case Screen::LOAD_GAME:
		//TODO
		break;
	case Screen::GAME:
		m_gameScreen->render(m_renderer);
		break;
	case Screen::BOTS:
		m_botsScreen->render(m_renderer);
		break;
	case Screen::STATISTICS:
		//TODO
		break;
	}

	SDL_RenderPresent(m_renderer); //Updates the screen
}


/**
* Deletes the used pointers. Quits SDL2, SDL2_image and SDL2_ttf.
*/
void NTTTManager::close(){

	TTF_CloseFont(m_movesFont);

	delete m_gameScreen;
	m_gameScreen = nullptr;

	delete m_newGameScreen;
	m_newGameScreen = nullptr;

	delete m_mainMenuScreen;
	m_mainMenuScreen = nullptr;

	delete m_botsScreen;
	m_botsScreen = nullptr;

	delete g_game;
	g_game = nullptr;

	delete m_checkMark;
	m_checkMark = nullptr;
	delete m_redCross;
	m_redCross = nullptr;
	delete m_blueCross;
	m_blueCross = nullptr;

	TTF_CloseFont(m_font);

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(g_window);
	m_renderer = nullptr;
	g_window = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

