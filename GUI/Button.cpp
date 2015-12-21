#include "Button.h"
#include "../NTTTManager.h"

Button::Button(const std::string text, unsigned int x, const unsigned int y){
	m_x = x;
	m_y = y;
	m_texture = new Texture(text, {0, 0, 0});
}
Button::~Button(){
	delete m_texture;
	m_texture = nullptr;
}

void Button::renderButton() const {
	m_texture->renderTexture(m_x + g_NtttManager.PADDING_X, m_y + g_NtttManager.PADDING_Y);
	SDL_Rect rect = { m_x, m_y, getWidth(), getHeight() };
	SDL_RenderDrawRect(g_NtttManager.g_renderer, &rect);
}

void Button::getSize(unsigned int& width, unsigned int& height) const{
	width = getWidth();
	height = getHeight();
}

const /* unsigned */ int Button::getWidth() const{
	return m_texture->getWidth() + g_NtttManager.PADDING_X * 2;
}

const /* unsigned */ int Button::getHeight() const{
	return g_NtttManager.g_textHeight + g_NtttManager.PADDING_Y * 2;
}

void Button::registerClickFunc(void(*action)()) {
	m_action = action;
}

void Button::click() const{
	if (m_action != nullptr)
		m_action();
}

const bool Button::isInside(const /* unsigned */ int& x, const /* unsigned */ int& y) const{
	return m_x <= x && m_x + getWidth() >= x && m_y <= y && m_y + getHeight() >= y;
}
