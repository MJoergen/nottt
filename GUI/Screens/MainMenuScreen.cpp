#include "MainMenuScreen.h"

#include <string>

//New Game Button handling
static void onClickNewGameStatic(void *data){
	MainMenuScreen *pMainMenuScreen = static_cast<MainMenuScreen *> (data);
	pMainMenuScreen->onClickNewGame();
}

void MainMenuScreen::onClickNewGame(){
	(*m_currentState) = NEW_GAME;
}

//Load Game Button handling
static void onClickLoadGameStatic(void *data){
	MainMenuScreen *pMainMenuScreen = static_cast<MainMenuScreen *> (data);
	pMainMenuScreen->onClickLoadGame();
}

void MainMenuScreen::onClickLoadGame(){
	(*m_currentState) = LOAD_GAME;
}

//Bots Button handling
static void onClickBotsStatic(void *data){
	MainMenuScreen *pMainMenuScreen = static_cast<MainMenuScreen *> (data);
	pMainMenuScreen->onClickBots();
}

void MainMenuScreen::onClickBots(){
	(*m_currentState) = BOTS;
}

//Statistics Button handling
static void onClickStatisticsStatic(void *data){
	MainMenuScreen *pMainMenuScreen = static_cast<MainMenuScreen *> (data);
	pMainMenuScreen->onClickStatistics();
}

void MainMenuScreen::onClickStatistics(){
	(*m_currentState) = STATISTICS;
}

//Exit Button handling
static void onClickExitStatic(void *data){
	MainMenuScreen *pMainMenuScreen = static_cast<MainMenuScreen *> (data);
	pMainMenuScreen->onClickExit();
}

void MainMenuScreen::onClickExit(){
	(*m_currentState) = EXIT;
}

MainMenuScreen::MainMenuScreen(ScreenState* currentState){
	m_currentState = currentState;
}

MainMenuScreen::~MainMenuScreen(){
	cleanUp();
}

void MainMenuScreen::update(SDL_Renderer *renderer) {
	
}

void MainMenuScreen::prepareForQuit(){

}


void MainMenuScreen::init(SDL_Renderer *renderer, TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height){

	const unsigned int halfWidth = width / 2;
	const unsigned int halfHeight = height / 2;
	const SDL_Color black = { 0, 0, 0, 255 };

	m_headlineText = new Text(renderer, "Main Menu", headlineFont, black, 0, HEADLINE_PADDING);
	m_headlineText->setX(halfWidth - m_headlineText->getWidth() / 2);

	m_exitButton = new Button(renderer, "Exit", guiFont, 0, 0);

	const unsigned int buttonsHeight = m_exitButton->getHeight() * 5 + PADDING_Y * 4;
	const unsigned int halfBuffonsHeight = buttonsHeight / 2;

	m_exitButton->set(halfWidth - m_exitButton->getWidth() / 2, halfHeight + halfBuffonsHeight - m_exitButton->getHeight());
	m_exitButton->registerClickFunc(onClickExitStatic, this);
	
	m_statisticsButton = new Button(renderer, "Statistics", guiFont, 0, 0);
	m_statisticsButton->set(halfWidth - m_statisticsButton->getWidth() / 2, m_exitButton->getY() - PADDING_Y - m_statisticsButton->getHeight());
	m_statisticsButton->registerClickFunc(onClickStatisticsStatic, this);

	m_botsButton = new Button(renderer, "Bots", guiFont, 0, 0);
	m_botsButton->set(halfWidth - m_botsButton->getWidth() / 2, m_statisticsButton->getY() - PADDING_Y - m_botsButton->getHeight());
	m_botsButton->registerClickFunc(onClickBotsStatic, this);

	m_loadGameButton = new Button(renderer, "Load Game", guiFont, 0, 0);
	m_loadGameButton->set(halfWidth - m_loadGameButton->getWidth() / 2, m_botsButton->getY() - PADDING_Y - m_loadGameButton->getHeight());
	m_loadGameButton->registerClickFunc(onClickLoadGameStatic, this);

	m_newGameButton = new Button(renderer, "New Game", guiFont, 0, 0);
	m_newGameButton->set(halfWidth - m_newGameButton->getWidth() / 2, m_loadGameButton->getY() - PADDING_Y - m_newGameButton->getHeight());
	m_newGameButton->registerClickFunc(onClickNewGameStatic, this);

}

void MainMenuScreen::render(SDL_Renderer* renderer) const{ // The renderer will be used later
	m_headlineText->renderText(renderer);

	m_newGameButton->renderButton(renderer);
	m_loadGameButton->renderButton(renderer);
	m_botsButton->renderButton(renderer);
	m_statisticsButton->renderButton(renderer);
	m_exitButton->renderButton(renderer);
}

void MainMenuScreen::input(const SDL_Event & event){
	if (event.type != SDL_MOUSEBUTTONDOWN)
		return;

	int x, y;
	SDL_GetMouseState(&x, &y);

	if (m_newGameButton->isInside(x, y))
		m_newGameButton->click();
	else if (m_loadGameButton->isInside(x, y))
		m_loadGameButton->click();
	else if (m_botsButton->isInside(x, y))
		m_botsButton->click();
	else if (m_statisticsButton->isInside(x, y))
		m_statisticsButton->click();
	else if (m_exitButton->isInside(x, y))
		m_exitButton->click();
}

void MainMenuScreen::cleanUp(){
	delete m_headlineText;
	m_headlineText = nullptr;

	delete m_newGameButton;
	m_newGameButton = nullptr;

	delete m_loadGameButton;
	m_loadGameButton = nullptr;

	delete m_botsButton;
	m_botsButton = nullptr;

	delete m_statisticsButton;
	m_statisticsButton = nullptr;

	delete m_exitButton;
	m_exitButton = nullptr;
}