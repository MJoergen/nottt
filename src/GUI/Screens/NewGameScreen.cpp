#include <climits>
#include <cstdlib>
#include "NewGameScreen.h"
#include "../../NTTTGame.h"

static void onClickNewGameReturn(void *data) {
    NewGameScreen *pNewGameScreen = static_cast<NewGameScreen *>(data);
    pNewGameScreen->onClickReturn();
}

void NewGameScreen::onClickReturn() {
    m_errorMessage    = "";
    m_errorChanged    = true;
    (*m_currentState) = MAIN_MENU;
}

static void onClickNewGameStart(void *data) {
    NewGameScreen *pNewGameScreen = static_cast<NewGameScreen *>(data);
    pNewGameScreen->onClickStart();
}

// The number fields only accept digits, and return "0" when empty.
// Numbers that are too large become INT_MAX.
static int toNumber(const std::string& content) {
    long long number = std::strtoll(content.c_str(), nullptr, 10); // Saturates at LLONG_MAX
    return number > INT_MAX ? INT_MAX : (int)number;
}

void NewGameScreen::onClickStart() {
    const int boardCount = toNumber(m_boardCountTextField->getContent());
    const int boardSize  = toNumber(m_boardSizeTextField->getContent());
    const int lineSize   = toNumber(m_lineSizeTextField->getContent());
    const int gameSeed   = toNumber(m_gameSeedTextField->getContent());

    m_errorMessage = NTTTGame::checkSettings(boardCount, boardSize, lineSize);
    if (m_errorMessage.empty() && boardCount > MAX_BOARD_COUNT)
        m_errorMessage = "Board count must be at most " + std::to_string(MAX_BOARD_COUNT);
    m_errorChanged = true;
    if (!m_errorMessage.empty())
        return;

    (*m_boardCount) = boardCount;
    (*m_boardSize)  = boardSize;
    (*m_lineSize)   = lineSize;
    (*m_gameSeed)   = gameSeed;

    (*m_manualMode) = m_manualModeCheckBox->isChecked();

    (*m_writeLog) = m_writeLogCheckBox->isChecked();
    (*m_logName)  = m_logNameTextField->getContent();

    (*m_currentState) = GAME;
}


NewGameScreen::NewGameScreen(NewGameData data) {
    m_currentState = data.currentState;

    m_boardCount = data.boardCount;
    m_boardSize  = data.boardSize;
    m_lineSize   = data.lineSize;
    m_gameSeed   = data.gameSeed;
    m_manualMode = data.manualMode;
    m_writeLog   = data.writeLog;
    m_logName    = data.logName;

    m_checkMark = data.checkMark;
}

NewGameScreen::~NewGameScreen() {
    cleanUp();
}

void NewGameScreen::update(SDL_Renderer *renderer) {
    m_boardCountTextField->update(renderer);
    m_boardSizeTextField->update(renderer);
    m_lineSizeTextField->update(renderer);
    m_gameSeedTextField->update(renderer);

    m_logNameTextField->update(renderer);

    if (m_errorChanged) {
        delete m_errorText;
        m_errorText = nullptr;
        if (!m_errorMessage.empty()) {
            const SDL_Color red = {255, 0, 0, 255};
            m_errorText         = new Text(renderer, m_errorMessage, m_guiFont, red, 0, m_startGameButton->getY() + m_startGameButton->getHeight() + PADDING_Y);
            m_errorText->setX(m_width / 2 - m_errorText->getWidth() / 2);
        }
        m_errorChanged = false;
    }
}

void NewGameScreen::prepareForQuit() {
}

void NewGameScreen::init(SDL_Renderer *renderer, TTF_Font *headlineFont, TTF_Font *guiFont, const unsigned int width, const unsigned int height) {

    const unsigned int halfWidth = width / 2;
    const SDL_Color black        = {0, 0, 0, 255};

    m_guiFont = guiFont;
    m_width   = width;

    m_headlineText = new Text(renderer, "New Game", headlineFont, black, 0, HEADLINE_PADDING);
    m_headlineText->setX(halfWidth - m_headlineText->getWidth() / 2);

    m_boardCountText = new Text(renderer, "Board Count: ", guiFont, black, 0, m_headlineText->getY() + m_headlineText->getHeight() + HEADLINE_PADDING + PADDING_Y);
    m_boardSizeText  = new Text(renderer, "Board Size: ", guiFont, black, 0, m_boardCountText->getY());
    m_lineSizeText   = new Text(renderer, "Line Size: ", guiFont, black, 0, m_boardCountText->getY());

    m_boardCountTextField = new TextField(renderer, TextField::NUMBER, std::to_string(*m_boardCount), guiFont, 0, m_boardCountText->getY(), 50, -1);
    m_boardSizeTextField  = new TextField(renderer, TextField::NUMBER, std::to_string(*m_boardSize), guiFont, 0, m_boardCountText->getY(), 30, 8);
    m_lineSizeTextField   = new TextField(renderer, TextField::NUMBER, std::to_string(*m_lineSize), guiFont, 0, m_boardCountText->getY(), 30, 8);

    const unsigned int firstLineWidth     = m_boardCountText->getWidth() + m_boardSizeText->getWidth() + m_lineSizeText->getWidth() + m_boardCountTextField->getWidth() + m_boardSizeTextField->getWidth() + m_lineSizeTextField->getWidth() + 5 * PADDING_X;
    const unsigned int halfFirstLineWidth = firstLineWidth / 2;

    m_boardCountText->setX(halfWidth - halfFirstLineWidth);
    m_boardCountTextField->setX(m_boardCountText->getX() + m_boardCountText->getWidth() + PADDING_X);

    m_boardSizeText->setX(m_boardCountTextField->getX() + m_boardCountTextField->getWidth() + PADDING_X);
    m_boardSizeTextField->setX(m_boardSizeText->getX() + m_boardSizeText->getWidth() + PADDING_X);

    m_lineSizeText->setX(m_boardSizeTextField->getX() + m_boardSizeTextField->getWidth() + PADDING_X);
    m_lineSizeTextField->setX(m_lineSizeText->getX() + m_lineSizeText->getWidth() + PADDING_X);

    m_gameSeedText = new Text(renderer, "Game Seed: ", guiFont, black, 0, m_boardCountTextField->getY() + m_boardCountTextField->getHeight() + PADDING_Y);

    m_gameSeedTextField = new TextField(renderer, TextField::NUMBER, std::to_string(*m_gameSeed), guiFont, 0, m_gameSeedText->getY(), 200, INT_MAX);

    const unsigned int secondLineWidth     = m_gameSeedText->getWidth() + m_gameSeedTextField->getWidth() + PADDING_X;
    const unsigned int halfSecondLineWidth = secondLineWidth / 2;

    m_gameSeedText->setX(halfWidth - halfSecondLineWidth);
    m_gameSeedTextField->setX(m_gameSeedText->getX() + m_gameSeedText->getWidth() + PADDING_X);


    m_manualModeText = new Text(renderer, "Manual Mode:", guiFont, black, 0, m_gameSeedTextField->getY() + m_gameSeedTextField->getHeight() + PADDING_Y);

    m_manualModeCheckBox = new CheckBox(*m_manualMode, 0, m_manualModeText->getY(), m_manualModeText->getHeight());

    const unsigned int thirdLineWidth     = m_manualModeText->getWidth() + m_manualModeCheckBox->getWidth() + PADDING_X;
    const unsigned int halfThirdLineWidth = thirdLineWidth / 2;

    m_manualModeText->setX(halfWidth - halfThirdLineWidth);
    m_manualModeCheckBox->setX(m_manualModeText->getX() + m_manualModeText->getWidth() + PADDING_X);

    m_writeLogText = new Text(renderer, "Write log: ", guiFont, black, 0, m_manualModeText->getY() + m_manualModeText->getHeight() + PADDING_Y);

    m_writeLogCheckBox = new CheckBox(*m_writeLog, 0, m_writeLogText->getY(), m_writeLogText->getHeight());

    const unsigned int fourthLineWidth     = m_writeLogText->getWidth() + m_writeLogCheckBox->getWidth() + PADDING_X;
    const unsigned int halfFourthLineWidth = fourthLineWidth / 2;

    m_writeLogText->setX(halfWidth - halfFourthLineWidth);
    m_writeLogCheckBox->setX(m_writeLogText->getX() + m_writeLogText->getWidth() + PADDING_X);

    m_logNameText = new Text(renderer, "Log Name: ", guiFont, black, 0, m_writeLogText->getY() + m_writeLogText->getHeight() + PADDING_Y);

    m_logNameTextField = new TextField(renderer, TextField::TEXT, "log.txt", guiFont, 0, m_logNameText->getY(), 200, -1);

    const unsigned int fifthLineWidth     = m_logNameText->getWidth() + m_logNameTextField->getWidth() + PADDING_X;
    const unsigned int halfFifthLineWidth = fifthLineWidth / 2;

    m_logNameText->setX(halfWidth - halfFifthLineWidth);
    m_logNameTextField->setX(m_logNameText->getX() + m_logNameText->getWidth() + PADDING_X);

    m_startGameButton = new Button(renderer, "Start Game", guiFont, 0, m_logNameTextField->getY() + m_logNameTextField->getHeight() + PADDING_Y);
    m_startGameButton->setX(halfWidth - m_startGameButton->getWidth() / 2);
    m_startGameButton->registerClickFunc(onClickNewGameStart, this);

    m_returnButton = new Button(renderer, "Back to Main Menu", guiFont, 0, 0);
    m_returnButton->set(halfWidth - m_returnButton->getWidth() / 2, height - PADDING_Y - m_returnButton->getHeight());
    m_returnButton->registerClickFunc(onClickNewGameReturn, this);
}

void NewGameScreen::render(SDL_Renderer *renderer) const {
    m_boardCountText->render(renderer);
    m_boardSizeText->render(renderer);
    m_lineSizeText->render(renderer);
    m_gameSeedText->render(renderer);

    m_boardCountTextField->render(renderer, SDL_GetTicks());
    m_boardSizeTextField->render(renderer, SDL_GetTicks());
    m_lineSizeTextField->render(renderer, SDL_GetTicks());
    m_gameSeedTextField->render(renderer, SDL_GetTicks());

    m_manualModeText->render(renderer);
    m_manualModeCheckBox->render(renderer, m_checkMark);

    m_startGameButton->render(renderer);
    if (m_errorText)
        m_errorText->render(renderer);

    m_writeLogText->render(renderer);

    m_writeLogCheckBox->render(renderer, m_checkMark);

    if (m_writeLogCheckBox->isChecked()) {
        m_logNameText->render(renderer);
        m_logNameTextField->render(renderer, SDL_GetTicks());
    }

    m_headlineText->render(renderer);

    m_returnButton->render(renderer);
}

void NewGameScreen::input(const SDL_Event& event) {
    if (event.type == SDL_TEXTINPUT) { // Responds to textinput
        if (m_boardCountTextField->isSelected())
            m_boardCountTextField->onTextInput(event.text.text);
        else if (m_boardSizeTextField->isSelected())
            m_boardSizeTextField->onTextInput(event.text.text);
        else if (m_lineSizeTextField->isSelected())
            m_lineSizeTextField->onTextInput(event.text.text);
        else if (m_gameSeedTextField->isSelected())
            m_gameSeedTextField->onTextInput(event.text.text);
        else if (m_logNameTextField->isSelected())
            m_logNameTextField->onTextInput(event.text.text);
    } else if (event.type == SDL_KEYDOWN) { // Responds to keyboardinput
        if (m_boardCountTextField->isSelected())
            m_boardCountTextField->onKeyPress(event.key.keysym);
        else if (m_boardSizeTextField->isSelected())
            m_boardSizeTextField->onKeyPress(event.key.keysym);
        else if (m_lineSizeTextField->isSelected())
            m_lineSizeTextField->onKeyPress(event.key.keysym);
        else if (m_gameSeedTextField->isSelected())
            m_gameSeedTextField->onKeyPress(event.key.keysym);
        else if (m_logNameTextField->isSelected())
            m_logNameTextField->onKeyPress(event.key.keysym);
    } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) { // Responds to left mouse clicks
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
            m_boardCountTextField->click(x);
        else if (m_boardSizeTextField->isInside(x, y))
            m_boardSizeTextField->click(x);
        else if (m_lineSizeTextField->isInside(x, y))
            m_lineSizeTextField->click(x);
        else if (m_gameSeedTextField->isInside(x, y))
            m_gameSeedTextField->click(x);
        else if (m_logNameTextField->isInside(x, y))
            m_logNameTextField->click(x);
        else if (m_startGameButton->isInside(x, y))
            m_startGameButton->click();
        else if (m_returnButton->isInside(x, y))
            m_returnButton->click();
        else if (m_manualModeCheckBox->isInside(x, y))
            m_manualModeCheckBox->toggle();
        else if (m_writeLogCheckBox->isInside(x, y))
            m_writeLogCheckBox->toggle();
    }
}

void NewGameScreen::cleanUp() {

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

    delete m_manualModeCheckBox;
    m_manualModeCheckBox = nullptr;

    delete m_startGameButton;
    m_startGameButton = nullptr;

    delete m_errorText;
    m_errorText = nullptr;

    delete m_writeLogText;
    m_writeLogText = nullptr;
    delete m_logNameText;
    m_logNameText = nullptr;

    delete m_writeLogCheckBox;
    m_writeLogCheckBox = nullptr;

    delete m_logNameTextField;
    m_logNameTextField = nullptr;

    delete m_headlineText;
    m_headlineText = nullptr;

    delete m_returnButton;
    m_returnButton = nullptr;
}
