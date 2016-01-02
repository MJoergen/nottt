#include "Button.h"
#include "../NTTTManager.h"

Button::Button(SDL_Renderer *renderer, const std::string text, TTF_Font *font, unsigned int x, const unsigned int y){
	m_x = x;
	m_y = y;
	m_texture = new Texture(renderer, text, font, {0, 0, 0});
}

Button::~Button(){
	delete m_texture;
	m_texture = nullptr;
}

void Button::renderButton(SDL_Renderer *renderer) const {
	m_texture->renderTexture(renderer, m_x + m_paddingX, m_y + m_paddingY);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect rect = { m_x, m_y, getWidth(), getHeight() };
	SDL_RenderDrawRect(renderer, &rect);
}

const int Button::getWidth() const{
	return m_texture->getWidth() + m_paddingX * 2;
}

const int Button::getHeight() const{
	return m_texture->getHeight() + m_paddingY * 2;
}

void Button::registerClickFunc(void(*action)(void *), void *data) {
	m_action = action;
    m_data = data;
}

void Button::click() const{
	if (m_action != nullptr)
		m_action(m_data);
}

const bool Button::isInside(const int& x, const int& y) const{
	return m_x <= x && m_x + getWidth() >= x && m_y <= y && m_y + getHeight() >= y;
}
