#include "NewGameScreen.h"

static void onClickNewGameReturn(void *data){
	NewGameScreen *pNewGameScreen = static_cast<NewGameScreen *> (data);
	pNewGameScreen->onClickReturn();
}

void NewGameScreen::onClickReturn(){
	(*m_currentState) = MAIN_MENU;
}

static void onClickNewGameStart(void *data){
	NewGameScreen *pNewGameScreen = static_cast<NewGameScreen *> (data);
	pNewGameScreen->onClickStart();
}

void NewGameScreen::onClickStart(){
	(*m_boardCount) = std::stoi(m_boardCountTextField->getContent());
	(*m_boardSize) = std::stoi(m_boardSizeTextField->getContent());
	(*m_lineSize) = std::stoi(m_lineSizeTextField->getContent());
	(*m_gameSeed) = std::stoi(m_gameSeedTextField->getContent());

	(*m_manualMode) = m_manualModeRadioButton->isChecked();
	
	(*m_writeLog) = m_writeLogRadioButton->isChecked();
	(*m_logName) = m_logNameTextField->getContent();
	
	(*m_currentState) = GAME;
}



NewGameScreen::NewGameScreen(NewGameData data){
	m_currentState = data.currentState;

	m_boardCount = data.boardCount;
	m_boardSize = data.boardSize;
	m_lineSize = data.lineSize;
	m_gameSeed = data.gameSeed;
	m_manualMode = data.manualMode;
	m_writeLog = data.writeLog;
	m_logName = data.logName;

	m_checkMark = data.checkMark;
}

NewGameScreen::~NewGameScreen(){
	cleanUp();
}

void NewGameScreen::update(SDL_Renderer *renderer) {
	m_boardCountTextField->updateTextField(renderer);
	m_boardSizeTextField->updateTextField(renderer);
	m_lineSizeTextField->updateTextField(renderer);
	m_gameSeedTextField->updateTextField(renderer);

	m_logNameTextField->updateTextField(renderer);
}

void NewGameScreen::prepareForQuit(){

}

void NewGameScreen::init(SDL_Renderer *renderer, TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height){

	const unsigned int halfWidth = width / 2;
	const SDL_Color black = { 0, 0, 0, 255 };

	m_headlineText = new Text(renderer, "New Game", headlineFont, black, 0, HEADLINE_PADDING);
	m_headlineText->setX(halfWidth - m_headlineText->getWidth() / 2);

	m_boardCountText = new Text(renderer, "Board Count: ", guiFont, black, 0, m_headlineText->getY() + m_headlineText->getHeight() + HEADLINE_PADDING + PADDING_Y);
	m_boardSizeText = new Text(renderer, "Board Size: ", guiFont, black, 0, m_boardCountText->getY());
	m_lineSizeText = new Text(renderer, "Line Size: ", guiFont, black, 0, m_boardCountText->getY());

	m_boardCountTextField = new TextField(renderer, TextField::NUMBER, std::to_string(*m_boardCount), guiFont, 0, m_boardCountText->getY(), 50, -1);
	m_boardSizeTextField = new TextField(renderer, TextField::NUMBER, std::to_string(*m_boardSize), guiFont, 0, m_boardCountText->getY(), 30, 8);
	m_lineSizeTextField = new TextField(renderer, TextField::NUMBER, std::to_string(*m_lineSize), guiFont, 0, m_boardCountText->getY(), 30, 8);

	const unsigned int firstLineWidth = m_boardCountText->getWidth() + m_boardSizeText->getWidth() + m_lineSizeText->getWidth()
		+ m_boardCountTextField->getWidth() + m_boardSizeTextField->getWidth() + m_lineSizeTextField->getWidth() + 5 * PADDING_X;
	const unsigned int halfFirstLineWidth = firstLineWidth / 2;

	m_boardCountText->setX(halfWidth - halfFirstLineWidth);
	m_boardCountTextField->setX(m_boardCountText->getX() + m_boardCountText->getWidth() + PADDING_X);

	m_boardSizeText->setX(m_boardCountTextField->getX() + m_boardCountTextField->getWidth() + PADDING_X);
	m_boardSizeTextField->setX(m_boardSizeText->getX() + m_boardSizeText->getWidth() + PADDING_X);

	m_lineSizeText->setX(m_boardSizeTextField->getX() + m_boardSizeTextField->getWidth() + PADDING_X);
	m_lineSizeTextField->setX(m_lineSizeText->getX() + m_lineSizeText->getWidth() + PADDING_X);
	
	//
	m_gameSeedText = new Text(renderer, "Game Seed: ", guiFont, black, 0, m_boardCountText->getY() + m_boardCountText->getHeight() + PADDING_Y);

	m_gameSeedTextField = new TextField(renderer, TextField::NUMBER, std::to_string(*m_gameSeed), guiFont, 0, m_gameSeedText->getY(), 200, INT_MAX);

	const unsigned int secondLineWidth = m_gameSeedText->getWidth() + m_gameSeedTextField->getWidth() + PADDING_X;
	const unsigned int halfSecondLineWidth = secondLineWidth / 2;

	m_gameSeedText->setX(halfWidth - halfSecondLineWidth);
	m_gameSeedTextField->setX(m_gameSeedText->getX() + m_gameSeedText->getWidth() + PADDING_X);


	//
	m_manualModeText = new Text(renderer, "Manual Mode:", guiFont, black, 0, m_gameSeedText->getY() + m_gameSeedText->getHeight() + PADDING_Y);

	m_manualModeRadioButton = new RadioButton(*m_manualMode, 0, m_manualModeText->getY(), m_manualModeText->getHeight());

	const unsigned int thirdLineWidth = m_manualModeText->getWidth() + m_manualModeRadioButton->getWidth() + PADDING_X;
	const unsigned int halfThirdLineWidth = thirdLineWidth / 2;

	m_manualModeText->setX(halfWidth - halfThirdLineWidth);
	m_manualModeRadioButton->setX(m_manualModeText->getX() + m_manualModeText->getWidth() + PADDING_X);

	//
	m_writeLogText = new Text(renderer, "Write log: ", guiFont, black, 0, m_manualModeText->getY() + m_manualModeText->getHeight() + PADDING_Y);

	m_writeLogRadioButton = new RadioButton(*m_writeLog, 0, m_writeLogText->getY(), m_writeLogText->getHeight());

	const unsigned int fourthLineWidth = m_writeLogText->getWidth() + m_writeLogRadioButton->getWidth() + PADDING_X;
	const unsigned int halfFourthLineWidth = fourthLineWidth / 2;

	m_writeLogText->setX(halfWidth - halfFourthLineWidth);
	m_writeLogRadioButton->setX(m_writeLogText->getX() + m_writeLogText->getWidth() + PADDING_X);

	//
	m_logNameText = new Text(renderer, "Log Name: ", guiFont, black, 0, m_writeLogText->getY() + m_writeLogText->getHeight() + PADDING_Y);

	m_logNameTextField = new TextField(renderer, TextField::TEXT, "log.txt", guiFont, 0, m_logNameText->getY(), 200, -1);

	const unsigned int fifthLineWidth = m_logNameText->getWidth() + m_logNameTextField->getWidth() + PADDING_X;
	const unsigned int halfFifthLineWidth = fifthLineWidth / 2;

	m_logNameText->setX(halfWidth - halfFifthLineWidth);
	m_logNameTextField->setX(m_logNameText->getX() + m_logNameText->getWidth() + PADDING_X);

	//
	m_startGameButton = new Button(renderer, "Start Game", guiFont, 0, m_logNameText->getY() + m_logNameText->getHeight() + PADDING_Y);
	m_startGameButton->setX(halfWidth - m_startGameButton->getWidth() / 2);
	m_startGameButton->registerClickFunc(onClickNewGameStart, this);

	//
	m_returnButton = new Button(renderer, "Back to Main Menu", guiFont, 0, 0);
	m_returnButton->set(halfWidth - m_returnButton->getWidth() / 2, height - PADDING_Y - m_returnButton->getHeight());
	m_returnButton->registerClickFunc(onClickNewGameReturn, this);

}

void NewGameScreen::render(SDL_Renderer* renderer) const{ // The renderer will be used later
	m_boardCountText->renderText(renderer);
	m_boardSizeText->renderText(renderer);
	m_lineSizeText->renderText(renderer);
	m_gameSeedText->renderText(renderer);

	m_boardCountTextField->renderTextField(renderer, SDL_GetTicks());
	m_boardSizeTextField->renderTextField(renderer, SDL_GetTicks());
	m_lineSizeTextField->renderTextField(renderer, SDL_GetTicks());
	m_gameSeedTextField->renderTextField(renderer, SDL_GetTicks());

	m_manualModeText->renderText(renderer);
	m_manualModeRadioButton->renderRadioButton(renderer, m_checkMark);

	m_startGameButton->renderButton(renderer);

	m_writeLogText->renderText(renderer);

	m_writeLogRadioButton->renderRadioButton(renderer, m_checkMark);

	if (m_writeLogRadioButton->isChecked()){
		m_logNameText->renderText(renderer);
		m_logNameTextField->renderTextField(renderer, SDL_GetTicks());
	}

	m_headlineText->renderText(renderer);

	m_returnButton->renderButton(renderer);
}

void NewGameScreen::input(const SDL_Event & event){
	if (event.type == SDL_TEXTINPUT){ //Responds to textinput
		if (m_boardCountTextField->isSelected())
			m_boardCountTextField->onKeyPress(event.key.keysym, event.text.text);
		else if (m_boardSizeTextField->isSelected())
			m_boardSizeTextField->onKeyPress(event.key.keysym, event.text.text);
		else if (m_lineSizeTextField->isSelected())
			m_lineSizeTextField->onKeyPress(event.key.keysym, event.text.text);
		else if (m_gameSeedTextField->isSelected())
			m_gameSeedTextField->onKeyPress(event.key.keysym, event.text.text);
		else if (m_logNameTextField->isSelected())
			m_logNameTextField->onKeyPress(event.key.keysym, event.text.text);
	}
	else if (event.type == SDL_KEYDOWN){ //Responds to keyboardinput
		if (m_boardCountTextField->isSelected())
			m_boardCountTextField->onKeyPress(event.key.keysym, "");
		else if (m_boardSizeTextField->isSelected())
			m_boardSizeTextField->onKeyPress(event.key.keysym, "");
		else if (m_lineSizeTextField->isSelected())
			m_lineSizeTextField->onKeyPress(event.key.keysym, "");
		else if (m_gameSeedTextField->isSelected())
			m_gameSeedTextField->onKeyPress(event.key.keysym, "");
		else if (m_logNameTextField->isSelected())
			m_logNameTextField->onKeyPress(event.key.keysym, "");
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN){ //Responds to mouseinput
		int x, y;
		SDL_GetMouseState(&x, &y);

		if (m_boardCountTextField->isSelected())
			m_boardCountTextField->deselect();
		if (m_boardSizeTextField->isSelected())
			m_boardSizeTextField->deselect();
		if (m_lineSizeTextField->isSelected())
			m_lineSizeTextField->deselect();
		if (m_gameSeedTextField->isSelected())
			m_gameSeedTextField->deselect();
		if (m_logNameTextField->isSelected())
			m_logNameTextField->deselect();

		if (m_boardCountTextField->isInside(x, y))
			m_boardCountTextField->select();
		else if (m_boardSizeTextField->isInside(x, y))
			m_boardSizeTextField->select();
		else if (m_lineSizeTextField->isInside(x, y))
			m_lineSizeTextField->select();
		else if (m_gameSeedTextField->isInside(x, y))
			m_gameSeedTextField->select();
		else if (m_logNameTextField->isInside(x, y))
			m_logNameTextField->select();
		else if (m_startGameButton->isInside(x, y))
			m_startGameButton->click();
		else if (m_returnButton->isInside(x, y))
			m_returnButton->click();
		else if (m_manualModeRadioButton->isInside(x, y))
			m_manualModeRadioButton->toggle();
		else if (m_writeLogRadioButton->isInside(x, y))
			m_writeLogRadioButton->toggle();
	}
}

void NewGameScreen::cleanUp(){

	delete m_boardCountText;
	m_boardCountText = nullptr;
	delete m_boardSizeText;
	m_boardSizeText = nullptr;
	delete m_lineSizeText;
	m_lineSizeText = nullptr;
	delete m_gameSeedText;
	m_gameSeedText = nullptr;

	delete m_boardCountTextField;
	m_boardCountTextField = nullptr;
	delete m_boardSizeTextField;
	m_boardSizeTextField = nullptr;
	delete m_lineSizeTextField;
	m_lineSizeTextField = nullptr;
	delete m_gameSeedTextField;
	m_gameSeedTextField = nullptr;

	delete m_manualModeText;
	m_manualModeText = nullptr;

	delete m_manualModeRadioButton;
	m_manualModeRadioButton = nullptr;

	delete m_startGameButton;
	m_startGameButton = nullptr;

	delete m_writeLogText;
	m_writeLogText = nullptr;
	delete m_logNameText;
	m_logNameText = nullptr;

	delete m_writeLogRadioButton;
	m_writeLogRadioButton = nullptr;

	delete m_logNameTextField;
	m_logNameTextField = nullptr;

	delete m_headlineText;
	m_headlineText = nullptr;

	delete m_returnButton;
	m_returnButton = nullptr;
}