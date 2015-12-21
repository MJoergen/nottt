#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>
#include "Texture.h"

class Button {
private:
	/* unsigned */ int m_x, m_y;

	Texture* m_texture = nullptr;
	
	void(*m_action)() = nullptr;

public:
	Button(const std::string text, const unsigned int x, const unsigned int y);
	virtual ~Button();

	void renderButton() const;
	
	void getSize(unsigned int& width, unsigned int& height) const;
	const /* unsigned */ int getWidth() const;
	const /* unsigned */ int getHeight() const;
	const /* unsigned */ int getX() const { return m_x; }
	const /* unsigned */ int getY() const { return m_y; }
	
	void click() const;
	void registerClickFunc(void(*action)());
	const bool isInside(const /* unsigned */ int& x, const /* unsigned */ int& y) const;

	friend std::ostream& operator <<(std::ostream &os, const Button &rhs){
		os << "Button: { X: " << rhs.m_x << ", Y: " << rhs.m_y << ", Width: " << rhs.getWidth()
			<< ", Height: " << rhs.getHeight() << " }" << std::endl;
		return os;
	}

};

#endif
