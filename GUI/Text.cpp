#include "Text.h"
#include "../NTTTManager.h"

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
	m_texture->renderTexture(renderer, m_x, m_y);
}

const unsigned int Text::getWidth() const{
	return m_texture->getWidth();
}

const unsigned int Text::getHeight() const{
	return m_texture->getHeight();
}
