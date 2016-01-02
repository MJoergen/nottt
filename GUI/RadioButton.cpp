#include "RadioButton.h"
#include "../NTTTManager.h"

RadioButton::RadioButton(const bool isChecked, const int x, const int y){
	m_isChecked = isChecked;
	m_x = x;
	m_y = y;
}

RadioButton::~RadioButton(){

}

const int RadioButton::getWidth() const {
	return g_NtttManager.g_textHeight + g_NtttManager.PADDING_Y * 2;
}

const int RadioButton::getHeight() const {
	return getWidth();
}

void RadioButton::renderRadioButton(SDL_Renderer *renderer, Texture *checkMark) const {
	if (m_isChecked)
		checkMark->renderTexture(renderer, m_x + g_NtttManager.PADDING_X, m_y + g_NtttManager.PADDING_Y,
			getWidth() - g_NtttManager.PADDING_X * 2, getHeight() - g_NtttManager.PADDING_Y * 2);
	SDL_Rect rect = { m_x, m_y, getWidth(), getHeight() };
	SDL_RenderDrawRect(renderer, &rect);
}

const bool RadioButton::isInside(const int& x, const int& y) const{
	return m_x <= x && m_x + getWidth() >= x && m_y <= y && m_y + getHeight() >= y;
}
