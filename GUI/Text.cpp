#include "Text.h"
#include "../NTTTManager.h"

Text::Text(const std::string text, const unsigned int x, const unsigned int y){
	m_x = x;
	m_y = y;
	m_texture = new Texture(text, { 0, 0, 0 });
}

Text::Text(const std::string text, const SDL_Color color, const unsigned int x, const unsigned int y){
	m_x = x;
	m_y = y;
	m_texture = new Texture(text, color);
}

Text::Text(const std::string text, TTF_Font* font, const SDL_Color color, const unsigned int x, const unsigned int y){
	m_x = x;
	m_y = y;
	m_texture = new Texture(text, font, color);
}

Text::~Text(){
	delete m_texture;
	m_texture = nullptr;
}

void Text::renderText() const{
	m_texture->renderTexture(m_x + g_NtttManager.PADDING_X, m_y + g_NtttManager.PADDING_Y);
}

const unsigned int Text::getWidth() const{
	return m_texture->getWidth() + g_NtttManager.PADDING_X * 2;
}

const unsigned int Text::getHeight() const{
	return m_texture->getHeight() + g_NtttManager.PADDING_Y * 2;
}
