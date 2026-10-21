#include <algorithm>
#include <cstdlib>
#include "TextField.h"
#include "../NTTTManager.h"

TextField::TextField(SDL_Renderer *renderer, const FieldType fieldType, const std::string content, TTF_Font *font, const int x, const int y, const int width, const int limit) {
    m_fieldType = fieldType;
    m_x         = x;
    m_y         = y;
    m_width     = width;
    m_limit     = limit;
    m_content   = content;
    m_font      = font;
    m_cursor    = m_content.length();
    updateCursorPixelX();
    genTexture(renderer);
}

void TextField::click(const int& x) {
    select();

    // Move the cursor to the character boundary nearest to the click.
    const int clickPos = x - m_x - PADDING_X + m_scroll;
    m_cursor           = 0;
    m_cursorPixelX     = 0;
    for (unsigned int i = 1; i <= m_content.length(); i++) {
        int w, h;
        TTF_SizeText(m_font, m_content.substr(0, i).c_str(), &w, &h);
        if (std::abs(w - clickPos) >= std::abs((int)m_cursorPixelX - clickPos))
            break;
        m_cursor       = i;
        m_cursorPixelX = w;
    }
    updateScroll();
}

TextField::~TextField() {
    delete m_texture;
    m_texture = nullptr;
}

void TextField::genTexture(SDL_Renderer *renderer) {
    delete m_texture;
    if (m_content.length() <= 0)
        m_texture = new Texture(renderer, " ", m_font, {0, 0, 0});
    else
        m_texture = new Texture(renderer, m_content, m_font, {0, 0, 0});
    m_changed = false;
}

void TextField::update(SDL_Renderer *renderer) {
    if (m_changed)
        genTexture(renderer);
}

void TextField::render(SDL_Renderer *renderer, const int& time) const {

    SDL_Rect rect = {m_x, m_y, m_width, (int)(m_texture->getHeight() + PADDING_Y * 2)};

    if (m_selected)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    else
        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);

    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderDrawRect(renderer, &rect);

    if (m_content.length() <= 0) {
        if (m_selected && time % 2000 >= 1000) {
            SDL_RenderDrawLine(renderer, m_x + PADDING_X, m_y + PADDING_Y, m_x + PADDING_X, m_y + m_texture->getHeight() + PADDING_Y);
        }
    } else {
        // Only draw the part of the text that fits inside the field.
        const int visibleWidth = std::min((int)m_texture->getWidth() - (int)m_scroll, m_width - PADDING_X * 2);
        const int height       = m_texture->getHeight();
        m_texture->render(renderer, m_scroll, 0, visibleWidth, height, m_x + PADDING_X, m_y + PADDING_Y, visibleWidth, height);
        if (m_selected && time % 2000 >= 1000) {
            const int cursorX = m_x + PADDING_X + m_cursorPixelX - m_scroll;
            SDL_RenderDrawLine(renderer, cursorX, m_y + m_texture->getHeight() + PADDING_Y, cursorX, m_y + PADDING_Y);
        }
    }
}

bool TextField::isInside(const int& x, const int& y) const {
    return m_x <= x && x < m_x + (int)getWidth() && m_y <= y && y < m_y + (int)getHeight();
}

static bool isAllDigits(const std::string& s) {
    return !s.empty() && s.find_first_not_of("0123456789") == std::string::npos;
}

void TextField::onKeyPress(const SDL_Keysym& keysym) {
    if (!m_selected)
        return;

    if (keysym.sym == SDLK_RETURN) {
        deselect();
        return;
    }

    if (keysym.sym == SDLK_BACKSPACE) {
        if (m_cursor > 0) {
            m_content.erase(--m_cursor, 1);
            updateCursorPixelX();
            m_changed = true;
        }
    }

    if (keysym.sym == SDLK_DELETE)
        if (m_cursor < m_content.length()) {
            m_content.erase(m_cursor, 1);
            updateScroll();
            m_changed = true;
        }

    if (keysym.sym == SDLK_LEFT)
        if (m_cursor > 0) {
            m_cursor--;
            updateCursorPixelX();
        }

    if (keysym.sym == SDLK_RIGHT)
        if (m_cursor < m_content.length()) {
            m_cursor++;
            updateCursorPixelX();
        }
}

void TextField::onTextInput(const std::string& text) {
    if (!m_selected)
        return;

    if (m_fieldType == NUMBER) {
        if (!isAllDigits(text))
            return;
        m_content.insert(m_cursor, text);
        if (m_limit >= 0 && std::stoll(m_content) > m_limit) {
            m_content.erase(m_cursor, 1);
        } else {
            m_cursor += text.length();
            updateCursorPixelX();
            m_changed = true;
        }
    } else {
        if (m_limit < 0 || m_content.length() + text.length() <= (unsigned)m_limit) {
            m_content.insert(m_cursor, text);
            m_cursor += text.length();
            updateCursorPixelX();
            m_changed = true;
        }
    }
}


void TextField::updateCursorPixelX() {
    if (m_cursor == 0)
        m_cursorPixelX = 0;
    else {
        int w, h;
        std::string content = m_content;
        content.resize(m_cursor);
        TTF_SizeText(m_font, content.c_str(), &w, &h);
        m_cursorPixelX = w;
    }
    updateScroll();
}

void TextField::updateScroll() {
    // Scroll as little as possible to keep the cursor visible, and don't leave
    // empty space on the right when the text is wider than the field.
    const int visibleWidth = m_width - PADDING_X * 2;
    int textWidth, h;
    TTF_SizeText(m_font, m_content.c_str(), &textWidth, &h);
    int scroll = std::min((int)m_scroll, std::max(0, textWidth - visibleWidth));
    scroll     = std::min(scroll, (int)m_cursorPixelX);
    scroll     = std::max(scroll, (int)m_cursorPixelX - visibleWidth);
    m_scroll   = scroll;
}

unsigned int TextField::getWidth() const {
    return m_width;
}

unsigned int TextField::getHeight() const {
    return m_texture->getHeight() + PADDING_Y * 2;
}
