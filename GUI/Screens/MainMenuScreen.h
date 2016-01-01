#ifndef _MAINMENU_SCREEN_H_
#define _MAINMENU_SCREEN_H_

class MainMenuScreen : public Screen {
public:
	virtual ~BotsScreen();
	virtual void init(ScreenState* currentState, TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height);
	virtual void render(const SDL_Renderer* renderer) const;
	virtual void input(const SDL_Event & e);
	virtual void úpdate();
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