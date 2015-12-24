#include "NTTTGame.h"
#include "NTTTPlayerIce.h"
#include "NTTTPlayerMike.h"
#include "NTTTManager.h"
#include <fstream>
#include <climits>

NTTTManager g_NtttManager;

static void onClickStatic(void *data)
{
	NTTTManager *pNtttManager = static_cast<NTTTManager *> (data);
	pNtttManager->onClick();
}

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

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED); //Creates the renderer used to render to the window
	if (g_renderer == NULL){
		std::cout << "Failed creating renderer: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(g_window);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	g_font = TTF_OpenFont(FONT_PATH.c_str(), FONT_SIZE); //Creates the font used in the application
	if (g_font == NULL){
		std::cout << "Failed loading font: " << TTF_GetError() << std::endl;
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return false;
	}
	g_textHeight = TTF_FontHeight(g_font);


	g_redCross = new Texture("RedCross.png");
	g_blueCross = new Texture("BlueCross.png");
	g_checkMark = new Texture("CheckMark.png");
	g_game = new NTTTGame();

	boardCountText = new Text("BoardCount: ", PADDING_X, PADDING_Y);
	boardCountTextField = new TextField(TextField::NUMBER, "3", PADDING_X + boardCountText->getWidth() + boardCountText->getX(), PADDING_Y, 60, -1);

	boardSizeText = new Text("BoardSize: ", PADDING_X + boardCountTextField->getWidth() + boardCountTextField->getX(), PADDING_Y);
	boardSizeTextField = new TextField(TextField::NUMBER, "4", PADDING_X + boardSizeText->getWidth() + boardSizeText->getX(), PADDING_Y, 30, 8);

	lineSizeText = new Text("LineSize: ", PADDING_X + boardSizeTextField->getWidth() + boardSizeTextField->getX(), PADDING_Y);
	lineSizeTextField = new TextField(TextField::NUMBER, "3", PADDING_X + lineSizeText->getWidth() + lineSizeText->getX(), PADDING_Y, 30, 8);

	startGameButton = new Button("Start Game", PADDING_X + lineSizeTextField->getWidth() + lineSizeTextField->getX(), PADDING_Y);

	startGameButton->registerClickFunc(onClickStatic, this);

	manualModeText = new Text("Manual Mode: ", PADDING_X, PADDING_Y * 2 + boardCountTextField->getHeight());
	manualModeRadioButton = new RadioButton(false, PADDING_X + manualModeText->getX() + manualModeText->getWidth(), manualModeText->getY());

	gameSeedText = new Text("Game Seed: ", PADDING_X + manualModeRadioButton->getX() + manualModeRadioButton->getWidth(), manualModeRadioButton->getY());
	gameSeedTextField = new TextField(TextField::NUMBER, "12345", PADDING_X + gameSeedText->getX() + gameSeedText->getWidth(), gameSeedText->getY(), 150, INT_MAX);

	logText = new Text("Write log: ", PADDING_X, manualModeText->getY() + PADDING_Y + manualModeText->getHeight());
	logRadioButton = new RadioButton(false, PADDING_X + logText->getX() + logText->getWidth(), logText->getY());


	m_gameInfoViewer = new GameInfoViewer();

	return true;
}

static int manageGameStatic( void* data)
{
    NTTTManager *pNtttManager = static_cast<NTTTManager *> (data);
    return pNtttManager->manageGame();
}

int NTTTManager::manageGame()
{

	srand(std::stoi(gameSeedTextField->getContent()));
	//This is where the game goes.

	//Some test code

	NTTTPlayerIce ice;
	NTTTPlayerMike mike;
	
	NTTTPlayer::OrderChoice iceOrderChoice, mikeOrderChoice;
	iceOrderChoice = ice.chooseOrder(*g_game);
	mikeOrderChoice = mike.chooseOrder(*g_game);

	if (iceOrderChoice == mikeOrderChoice){
		if (rand() % 2 == 0){ //TODO: Make this random by srand() or other method.
			std::cout << "Ice starts" << std::endl;
			m_player1 = &ice;
			m_player2 = &mike;
		}
		else {
			std::cout << "Mike starts" << std::endl;
			m_player1 = &mike;
			m_player2 = &ice;
		}
	}
	else if (iceOrderChoice == NTTTPlayer::FIRST || mikeOrderChoice == NTTTPlayer::LAST){
		std::cout << "Ice starts" << std::endl;
		m_player1 = &ice;
		m_player2 = &mike;
	}
	else{
		std::cout << "Mike starts" << std::endl;
		m_player1 = &mike;
		m_player2 = &ice;
	}

	initGraphics = true;

	int player = 1;
	NTTTMove move(0, 0, 0);
    bool gameActive = true;
    while (!quit)
	{
        if (gameActive)
        {
            switch (player)
            {
                case 1:
                    move = m_player1->performMove(*g_game);
                    g_game->makeMove(move, NTTTBoard::RED);
                    break;
                case 2:
                    move = m_player2->performMove(*g_game);
                    g_game->makeMove(move, NTTTBoard::BLUE);
                    break;
            }

			m_moves.push_back(move);

			m_gameInfoViewer->addMove(move);
			player = 3 - player;

            if (!g_game->isActive())
            {
                std::cout << "Player " << player << " won!" << std::endl;
                gameActive = false;
				m_gameInfoViewer->setWinner(player);
            }
        }
		if (manualModeRadioButton->isChecked()){
			do{
				SDL_Delay(100);
				if (backward){
					if (m_moves.size() <= 1)
						continue;
					g_game->undoMove(m_moves[m_moves.size() - 1]);
					m_moves.pop_back();
					player = 3 - player;
					backward = false;
				}
			}
			while (!forward && !quit);
			forward = false;
		}
		else{
			SDL_Delay(1000);
		}
	}
	
	if (logRadioButton->isChecked())
		writeLog(player);

	//isGameThreadRunning must be set to false just before the end.
	isGameThreadRunning = false;

	return player;
} // end of manageGame

void NTTTManager::writeLog(const int winner) const{
	//TODO

	std::ofstream outputFile;
	outputFile.open("log.txt");

	if (!outputFile.good()){
		std::cout << "Failed to open log.txt" << std::endl;
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
	outputFile << "# Moves:" << std::endl;
	for (unsigned int index = 0; index < m_moves.size(); index++){
		outputFile << m_moves[index].getBoardNumber() << " : (" << m_moves[index].getSquareX() << ", " << m_moves[index].getSquareY() << ")" << std::endl;
	}
	outputFile << "# Winner:" << std::endl;
	outputFile << "Winner: Player " << winner << std::endl;

	outputFile.close();

}

void NTTTManager::onClick()
{ //Function called when the start game button is pressed
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
	gameThread = SDL_CreateThread(manageGameStatic, "Game Thread", this);
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
void NTTTManager::loop(){
	
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
				else if (gameSeedTextField->isSelected())
					gameSeedTextField->onKeyPress(event.key.keysym, event.text.text);
			}
			else if (event.type == SDL_KEYDOWN){ //Responds to keyboardinput
				if (isStarted && manualModeRadioButton->isChecked()){
					if (event.key.keysym.sym == SDLK_RIGHT)
						forward = true;
					else if (event.key.keysym.sym == SDLK_LEFT)
							backward = true;
				}
				else if (boardCountTextField->isSelected())
					boardCountTextField->onKeyPress(event.key.keysym, "");
				else if (boardSizeTextField->isSelected())
					boardSizeTextField->onKeyPress(event.key.keysym, "");
				else if (lineSizeTextField->isSelected())
					lineSizeTextField->onKeyPress(event.key.keysym, "");
				else if (gameSeedTextField->isSelected())
					gameSeedTextField->onKeyPress(event.key.keysym, "");
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
				if (gameSeedTextField->isSelected())
					gameSeedTextField->deselect();

				if (boardCountTextField->isInside(x, y))
					boardCountTextField->select();
				else if (boardSizeTextField->isInside(x, y))
					boardSizeTextField->select();
				else if (lineSizeTextField->isInside(x, y))
					lineSizeTextField->select();
				else if (gameSeedTextField->isInside(x, y))
					gameSeedTextField->select();
				else if (startGameButton->isInside(x, y))
					startGameButton->click();
				else if (manualModeRadioButton->isInside(x, y))
					manualModeRadioButton->toggle();
				else if (logRadioButton->isInside(x, y))
					logRadioButton->toggle();
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

			if (initGraphics){
				m_gameInfoViewer->init();
				initGraphics = false;
			}
			
			for (int index = 0; index < g_game->getBoardCount(); index++){

				int boardX = BOARD_PADDING * (index % gridSize + 1) + boardRenderSize * (index % gridSize);
				int boardY = BOARD_PADDING * (int)(index / gridSize + 1) + boardRenderSize * (int)(index / gridSize);
				int squareSize = boardRenderSize / g_game->getBoardSize();

				if (m_moves.size() > 0){
					NTTTMove& move = m_moves[m_moves.size() - 1];
					if (move.getBoardNumber() == index){
						SDL_SetRenderDrawColor(g_renderer, 190, 190, 190, 255); //Color of the "last move" square
						SDL_Rect rect = { boardX + squareSize * move.getSquareX(), boardY + squareSize * move.getSquareY(), squareSize, squareSize };
						SDL_RenderFillRect(g_renderer, &rect);
					}
				}

				g_game->getBoards()[index].renderBoard(boardX, boardY, boardRenderSize);
			}

			for (unsigned int index = 0; index < g_moves.size(); index++){
				g_moves[index]->renderText();
			}

			m_gameInfoViewer->renderGameInfoViewer();
		}
		else {
			boardCountText->renderText();
			boardSizeText->renderText();
			lineSizeText->renderText();
			gameSeedText->renderText();

			boardCountTextField->renderTextField(SDL_GetTicks());
			boardSizeTextField->renderTextField(SDL_GetTicks());
			lineSizeTextField->renderTextField(SDL_GetTicks());
			gameSeedTextField->renderTextField(SDL_GetTicks());

			startGameButton->renderButton();

			manualModeText->renderText();
			manualModeRadioButton->renderRadioButton();

			logText->renderText();
			logRadioButton->renderRadioButton();
		}

		SDL_RenderPresent(g_renderer); //Updates the screen

		SDL_Delay(50); //Sleeps
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
void NTTTManager::close(){
	
	delete gameSeedText;
	gameSeedText = nullptr;
	delete gameSeedTextField;
	gameSeedTextField = nullptr;
	
	delete m_gameInfoViewer;
	m_gameInfoViewer = nullptr;

	delete g_game;
	g_game = nullptr;

	delete g_checkMark;
	g_checkMark = nullptr;
	delete g_redCross;
	g_redCross = nullptr;
	delete g_blueCross;
	g_blueCross = nullptr;

	delete logText;
	logText = nullptr;
	delete logRadioButton;
	logRadioButton = nullptr;

	delete manualModeText;
	manualModeText = nullptr;
	delete manualModeRadioButton;
	manualModeRadioButton = nullptr;

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

