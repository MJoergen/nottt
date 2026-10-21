#include "CheckBox.h"
#include "../NTTTManager.h"

CheckBox::CheckBox(const bool isChecked, const int x, const int y, const int size) {
    m_isChecked = isChecked;
    m_x         = x;
    m_y         = y;
    m_size      = size;
}

CheckBox::~CheckBox() {
}

int CheckBox::getWidth() const {
    return m_size;
}

int CheckBox::getHeight() const {
    return getWidth();
}

void CheckBox::render(SDL_Renderer *renderer, Texture *checkMark) const {
    if (m_isChecked)
        checkMark->render(renderer, m_x + PADDING_X, m_y + PADDING_Y,
                          getWidth() - PADDING_X * 2, getHeight() - PADDING_Y * 2);
    SDL_Rect rect = {m_x, m_y, getWidth(), getHeight()};
    SDL_RenderDrawRect(renderer, &rect);
}

bool CheckBox::isInside(const int& x, const int& y) const {
    return m_x <= x && x < m_x + getWidth() && m_y <= y && y < m_y + getHeight();
}
