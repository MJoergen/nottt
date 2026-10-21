#ifndef NEWGAME_SCREEN_H
#define NEWGAME_SCREEN_H

#include "Screen.h"
#include "../Text.h"
#include "../TextField.h"
#include "../Button.h"
#include "../CheckBox.h"


class NewGameScreen : public Screen {
public:
    struct NewGameData {
        ScreenState *currentState;
        int *boardCount;
        int *boardSize;
        int *lineSize;
        int *gameSeed;
        bool *manualMode;
        bool *writeLog;
        std::string *logName;

        Texture *checkMark;
    };
    NewGameScreen(NewGameData data);
    virtual ~NewGameScreen();
    virtual void init(SDL_Renderer *renderer, TTF_Font *headlineFont, TTF_Font *guiFont, const unsigned int width, const unsigned int height);
    virtual void render(SDL_Renderer *renderer) const;
    virtual void input(const SDL_Event& e);
    virtual void update(SDL_Renderer *renderer);
    virtual void prepareForQuit();
    virtual void cleanUp();
    void onClickReturn();
    void onClickStart();

private:
    const int MAX_BOARD_COUNT = 25; // More boards do not fit on the game screen


    int *m_boardCount = nullptr, *m_boardSize = nullptr, *m_lineSize = nullptr, *m_gameSeed = nullptr;
    bool *m_manualMode = nullptr, *m_writeLog = nullptr;
    std::string *m_logName = nullptr;

    Texture *m_checkMark;

    Text *m_headlineText   = nullptr;
    Text *m_boardCountText = nullptr, *m_boardSizeText = nullptr, *m_lineSizeText = nullptr, *m_gameSeedText = nullptr;
    TextField *m_boardCountTextField = nullptr, *m_boardSizeTextField = nullptr, *m_lineSizeTextField = nullptr, *m_gameSeedTextField = nullptr;
    Text *m_manualModeText         = nullptr;
    CheckBox *m_manualModeCheckBox = nullptr;
    Button *m_startGameButton      = nullptr;
    Text *m_writeLogText = nullptr, *m_logNameText = nullptr;
    CheckBox *m_writeLogCheckBox  = nullptr;
    TextField *m_logNameTextField = nullptr;
    Button *m_returnButton        = nullptr;

    TTF_Font *m_guiFont  = nullptr;
    unsigned int m_width = 0;
    std::string m_errorMessage; // Shown when the game settings are invalid
    bool m_errorChanged = false;
    Text *m_errorText   = nullptr;
};

#endif