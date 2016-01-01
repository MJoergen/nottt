#ifndef _BOTS_SCREEN_H_
#define _BOTS_SCREEN_H_

#include "Screen.h"
#include "../Text.h"
#include "../Button.h"
#include <vector>

class BotsScreen : public Screen {
public:
	virtual ~BotsScreen();
	virtual void init(ScreenState* currentState, TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height);
	virtual void render(const SDL_Renderer* renderer) const;
	virtual void input(const SDL_Event & e);
	virtual void úpdate();
	virtual void cleanUp();
	void onClick();
private:
	Text* m_headline = nullptr;
	Text* m_availableBotsText = nullptr;
	std::vector<Text*> m_botsTexts;
	Button* m_return = nullptr;
};

#endif