#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include <SDL.h>
#include "Texture.h"

class CheckBox {
private:
    int m_x, m_y;
    bool m_isChecked = false;
    int m_size;

    const int PADDING_X = 3, PADDING_Y = 3;

public:
    CheckBox(const bool isChecked, const int x, const int y, const int size);
    virtual ~CheckBox();


    void render(SDL_Renderer *renderer, Texture *checkMark) const;

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

    bool isChecked() { return m_isChecked; }
    void check() { m_isChecked = true; }
    void uncheck() { m_isChecked = false; }
    void toggle() { m_isChecked = !m_isChecked; }

    bool isInside(const int& x, const int& y) const;

    friend std::ostream& operator<<(std::ostream& os, const CheckBox& rhs) {
        os << "CheckBox: { isChecked: " << rhs.m_isChecked << ", X: " << rhs.m_x << ", Y: " << rhs.m_y << ", Width: " << rhs.getWidth()
           << ", Height: " << rhs.getHeight() << " }" << std::endl;
        return os;
    }
};

#endif
