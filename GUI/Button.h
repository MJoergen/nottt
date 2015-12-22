#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>
#include "Texture.h"

class Button {
private:
	int m_x, m_y;

	Texture* m_texture = nullptr;
	
	void(*m_action)(void *) = nullptr;
    void *m_data;

public:
	Button(const std::string text, const unsigned int x, const unsigned int y);
	virtual ~Button();

	void renderButton() const;
	
	const int getWidth() const;
	const int getHeight() const;
	const int getX() const { return m_x; }
	const int getY() const { return m_y; }
	
	void click() const;
	void registerClickFunc(void(*action)(void*), void*);
	const bool isInside(const /* unsigned */ int& x, const /* unsigned */ int& y) const;

	friend std::ostream& operator <<(std::ostream &os, const Button &rhs){
		os << "Button: { X: " << rhs.m_x << ", Y: " << rhs.m_y << ", Width: " << rhs.getWidth()
			<< ", Height: " << rhs.getHeight() << " }" << std::endl;
		return os;
	}

};

#endif
