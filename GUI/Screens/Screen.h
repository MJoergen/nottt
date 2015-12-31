#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <SDL.h>
#include <SDL_ttf.h>

class Screen {
public:

	enum ScreenState{
		MAIN_MENU, NEW_GAME, LOAD_GAME, STATISTICS, BOTS
	};

	virtual ~Screen() {};
	virtual void init(ScreenState* currentState, TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height) = 0;
	virtual void render(const SDL_Renderer* renderer) const = 0;
	virtual void input(const SDL_Event* e) = 0;
	virtual void cleanUp() = 0;
protected:
	const unsigned int HEADLINE_PADDING = 10;
	ScreenState* m_currentState = nullptr;
};

#endif