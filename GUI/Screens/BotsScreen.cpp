#include "BotsScreen.h"
#include <string>
#include "../../NTTTPlayerMike.h"
#include "../../NTTTPlayerIce.h"

static void onClickReturn(void *data){
	BotsScreen *pBotsScreen = static_cast<BotsScreen *> (data);
	pBotsScreen->onClick();
}

void BotsScreen::onClick(){
	(*m_currentState) = MAIN_MENU;
}

BotsScreen::~BotsScreen(){
	cleanUp();
}

void BotsScreen::init(ScreenState* currentState, TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height){
	m_currentState = currentState;

	const unsigned int halfWidth = width / 2;
	const SDL_Color black = { 0, 0, 0, 255 };

	m_headline = new Text("Bots", headlineFont, black, 0, HEADLINE_PADDING);
	m_headline->setX(halfWidth - m_headline->getWidth()/2);

	m_availableBotsText = new Text("Available Bots: ", guiFont, black, PADDING_X, m_headline->getY() + m_headline->getHeight() + HEADLINE_PADDING + PADDING_Y);

	// This part might need changing ------>

	NTTTPlayerMike mikeBot;
	Text* mikeBotText = new Text(mikeBot.getName(), guiFont, black, m_availableBotsText->getX() + PADDING_X, m_availableBotsText->getY() + m_availableBotsText->getHeight() + PADDING_Y);

	NTTTPlayerIce iceBot;
	Text* iceBotText = new Text(iceBot.getName(), guiFont, black, mikeBotText->getX(), mikeBotText->getY() + mikeBotText->getHeight() + PADDING_Y);

	m_botsTexts.push_back(mikeBotText);
	m_botsTexts.push_back(iceBotText);

	m_return = new Button("Back to Main Menu", 0, height - PADDING_Y);
	m_return->setX(halfWidth - m_return->getWidth() / 2);
	m_return->registerClickFunc(onClickReturn, this);

	// <-----


}

void BotsScreen::render(const SDL_Renderer* renderer) const{ // The renderer will be used later
	m_availableBotsText->renderText();

	for (unsigned int index = 0; index < m_botsTexts.size(); index++){
		m_botsTexts[index]->renderText();
	}

	m_return->renderButton();
}

void BotsScreen::input(const SDL_Event* e){
	if (e->type != SDL_MOUSEBUTTONDOWN)
		return;

	int x, y;
	SDL_GetMouseState(&x, &y);
	
	if (m_return->isInside(x, y))
		m_return->click();
}

void BotsScreen::cleanUp(){
	delete m_headline;
	m_headline = nullptr;

	delete m_availableBotsText;
	m_availableBotsText = nullptr;

	for (unsigned int index = 0; index < m_botsTexts.size(); index++){
		delete m_botsTexts[index];
	}
	m_botsTexts.resize(0);

	delete m_return;
	m_return = nullptr;
}