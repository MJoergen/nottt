#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include "Texture.h"

class Button {
private:
    int m_x, m_y;

    Texture *m_texture = nullptr;

    void (*m_action)(void *) = nullptr;
    void *m_data;

    const int PADDING_X = 3, PADDING_Y = 5;

public:
    Button(SDL_Renderer *renderer, const std::string text, TTF_Font *font, const unsigned int x, const unsigned int y);
    virtual ~Button();
    Button(const Button&)            = delete;
    Button& operator=(const Button&) = delete;

    void render(SDL_Renderer *renderer) const;

    int getWidth() const;
    int getHeight() const;
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    void setX(const unsigned int x) { m_x = x; }
    void setY(const unsigned int y) { m_y = y; }
    void set(const unsigned int x, const unsigned int y) {
        m_x = x;
        m_y = y;
    }

    void click() const;
    void registerClickFunc(void (*action)(void *), void *);
    bool isInside(const int& x, const int& y) const;

    friend std::ostream& operator<<(std::ostream& os, const Button& rhs) {
        os << "Button: { X: " << rhs.m_x << ", Y: " << rhs.m_y << ", Width: " << rhs.getWidth()
           << ", Height: " << rhs.getHeight() << " }" << std::endl;
        return os;
    }
};

#endif
