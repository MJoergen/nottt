#ifndef _MAINMENU_SCREEN_H_
#define _MAINMENU_SCREEN_H_

#include "Screen.h"
#include "../Text.h"
#include "../Button.h"

class MainMenuScreen : public Screen {
public:
	MainMenuScreen(ScreenState* currentState);
	virtual ~MainMenuScreen();
	virtual void init(SDL_Renderer *renderer, TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height);
	virtual void render(SDL_Renderer* renderer) const;
	virtual void input(const SDL_Event & e);
	virtual void update(SDL_Renderer *renderer);
	virtual void prepareForQuit();
	virtual void cleanUp();
	void onClickNewGame();
	void onClickLoadGame();
	void onClickBots();
	void onClickStatistics();
	void onClickExit();
private:
	Text* m_headlineText = nullptr;
	Button *m_newGameButton = nullptr, *m_loadGameButton = nullptr, *m_botsButton = nullptr, *m_statisticsButton = nullptr, *m_exitButton = nullptr;
};

#endif