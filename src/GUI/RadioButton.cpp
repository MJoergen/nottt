#include "RadioButton.h"
#include "../NTTTManager.h"

RadioButton::RadioButton(const bool isChecked, const int x, const int y, const int size) {
    m_isChecked = isChecked;
    m_x         = x;
    m_y         = y;
    m_size      = size;
}

RadioButton::~RadioButton() {
}

int RadioButton::getWidth() const {
    return m_size;
}

int RadioButton::getHeight() const {
    return getWidth();
}

void RadioButton::render(SDL_Renderer *renderer, Texture *checkMark) const {
    if (m_isChecked)
        checkMark->render(renderer, m_x + PADDING_X, m_y + PADDING_Y,
                          getWidth() - PADDING_X * 2, getHeight() - PADDING_Y * 2);
    SDL_Rect rect = {m_x, m_y, getWidth(), getHeight()};
    SDL_RenderDrawRect(renderer, &rect);
}

bool RadioButton::isInside(const int& x, const int& y) const {
    return m_x <= x && x < m_x + getWidth() && m_y <= y && y < m_y + getHeight();
}
