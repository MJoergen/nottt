#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "Texture.h"

class Text {
private:
    unsigned int m_x, m_y;
    Texture *m_texture;

public:
    Text(SDL_Renderer *renderer, const std::string text, TTF_Font *font, const SDL_Color color, const unsigned int x, const unsigned int y);
    virtual ~Text();
    Text(const Text&)            = delete;
    Text& operator=(const Text&) = delete;
    void render(SDL_Renderer *renderer) const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getX() const { return m_x; }
    unsigned int getY() const { return m_y; }
    void setX(const unsigned int x) { m_x = x; }
    void setY(const unsigned int y) { m_y = y; }
    void set(const unsigned int x, const unsigned int y) {
        m_x = x;
        m_y = y;
    }

    friend std::ostream& operator<<(std::ostream& os, const Text& rhs) {
        os << "Text: { X: " << rhs.m_x << ", Y: " << rhs.m_y << ", Width: " << rhs.getWidth()
           << ", Height: " << rhs.getHeight() << " }" << std::endl;
        return os;
    }
};

#endif
