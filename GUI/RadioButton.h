#ifndef _RADIOBUTTON_H_
#define _RADIOBUTTON_H_

#include <SDL.h>
#include "Texture.h"

class RadioButton {
private:
	
	int m_x, m_y;
	
	bool m_isChecked = false;

public:
	RadioButton(const bool isChecked, const int x, const int y);
	virtual ~RadioButton();


	void renderRadioButton() const;

	void getSize(int& width, int& height) const;
	const int getWidth() const;
	const int getHeight() const;
	const int getX() const { return m_x; }
	const int getY() const { return m_y; }
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