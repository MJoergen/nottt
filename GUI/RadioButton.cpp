#include "RadioButton.h"
#include "../NTTTManager.h"

RadioButton::RadioButton(const bool isChecked, const int x, const int y){
	m_isChecked = isChecked;
	m_x = x;
	m_y = y;
}

const int RadioButton::getWidth() const {
	return g_NtttManager.g_textHeight + g_NtttManager.PADDING_Y * 2;
}

const int RadioButton::getHeight() const {
	return getWidth();
}

void RadioButton::renderRadioButton() const {
	g_NtttManager.g_checkMark->renderTexture(m_x + g_NtttManager.PADDING_X, m_y + g_NtttManager.PADDING_Y,
		getWidth() - g_NtttManager.PADDING_X * 2, getHeight() - g_NtttManager.PADDING_Y * 2);
	SDL_Rect rect = { m_x, m_y, getWidth(), getHeight() };
	SDL_RenderDrawRect(g_NtttManager.g_renderer, &rect);
}

void RadioButton::getSize(int& width, int& height) const{
	width = getWidth();
	height = getHeight();
}

const bool Button::isInside(const int& x, const int& y) const{
	return m_x <= x && m_x + getWidth() >= x && m_y <= y && m_y + getHeight() >= y;
}
