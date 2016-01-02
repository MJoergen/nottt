#ifndef _RADIOBUTTON_H_
#define _RADIOBUTTON_H_

#include <SDL.h>
#include "Texture.h"

class RadioButton {
private:
	
	int m_x, m_y;
	bool m_isChecked = false;
	int m_size;

	const int m_paddingX = 3, m_paddingY = 3;

public:
	RadioButton(const bool isChecked, const int x, const int y, const int size);
	virtual ~RadioButton();


	void renderRadioButton(SDL_Renderer *renderer, Texture *checkMark) const;

	const int getWidth() const;
	const int getHeight() const;
	const int getX() const { return m_x; }
	const int getY() const { return m_y; }
	void setX(const unsigned int x) { m_x = x; }
	void setY(const unsigned int y) { m_y = y; }
	void set(const unsigned int x, const unsigned int y) { m_x = x; m_y = y; }

	const bool isChecked() { return m_isChecked; }
	void check() { m_isChecked = true; }
	void uncheck() { m_isChecked = false; }
	void toggle() { m_isChecked = !m_isChecked; }

	const bool isInside(const int& x, const int& y) const;

	friend std::ostream& operator <<(std::ostream &os, const RadioButton &rhs){
		os << "RadioButton: { isChecked: " << rhs.m_isChecked << ", X: " << rhs.m_x << ", Y: " << rhs.m_y << ", Width: " << rhs.getWidth()
			<< ", Height: " << rhs.getHeight() << " }" << std::endl;
		return os;
	}
};

#endif
