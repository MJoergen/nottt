#include "RadioButton.h"
#include "../NTTTManager.h"

RadioButton::RadioButton(const bool isChecked, const int x, const int y, const int size){
	m_isChecked = isChecked;
	m_x = x;
	m_y = y;
	m_size = size;
}

RadioButton::~RadioButton(){

}

const int RadioButton::getWidth() const {
	return m_size;
}

const int RadioButton::getHeight() const {
	return getWidth();
}

void RadioButton::renderRadioButton(SDL_Renderer *renderer, Texture *checkMark) const {
	if (m_isChecked)
		checkMark->renderTexture(renderer, m_x + m_paddingX, m_y + m_paddingY,
		getWidth() - m_paddingX * 2, getHeight() - m_paddingY * 2);
	SDL_Rect rect = { m_x, m_y, getWidth(), getHeight() };
	SDL_RenderDrawRect(renderer, &rect);
}

const bool RadioButton::isInside(const int& x, const int& y) const{
	return m_x <= x && m_x + getWidth() >= x && m_y <= y && m_y + getHeight() >= y;
}
