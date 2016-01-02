#include "Text.h"
#include "../NTTTManager.h"

Text::Text(SDL_Renderer *renderer, const std::string text, const unsigned int x, const unsigned int y){
	m_x = x;
	m_y = y;
	m_texture = new Texture(renderer, text, { 0, 0, 0 });
}

Text::Text(SDL_Renderer *renderer, const std::string text, const SDL_Color color, const unsigned int x, const unsigned int y){
	m_x = x;
	m_y = y;
	m_texture = new Texture(renderer, text, color);
}

Text::Text(SDL_Renderer *renderer, const std::string text, TTF_Font* font, const SDL_Color color, const unsigned int x, const unsigned int y){
	m_x = x;
	m_y = y;
	m_texture = new Texture(renderer, text, font, color);
}

Text::~Text(){
	delete m_texture;
	m_texture = nullptr;
}

void Text::renderText(SDL_Renderer *renderer) const{
	m_texture->renderTexture(renderer, m_x + g_NtttManager.PADDING_X, m_y + g_NtttManager.PADDING_Y);
}

const unsigned int Text::getWidth() const{
	return m_texture->getWidth() + g_NtttManager.PADDING_X * 2;
}

const unsigned int Text::getHeight() const{
	return m_texture->getHeight() + g_NtttManager.PADDING_Y * 2;
}
