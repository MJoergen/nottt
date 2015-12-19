#ifndef _TEXT_H_
#define _TEXT_H_

#include "Texture.h"
#include <string>

extern const int PADDING_X, PADDING_Y;
extern int g_textHeight;

class Text {
private:
	unsigned int m_x, m_y;
	Texture* m_texture;
public:
	Text(const std::string text, unsigned int x, const unsigned int y);
	virtual ~Text();
	void renderText() const;
	void getSize(unsigned int& width, unsigned int& height) const;
	const unsigned int getWidth() const;
	const unsigned int getHeight() const;
	const unsigned int getX() const { return m_x; }
	const unsigned int getY() const { return m_y; }
	friend std::ostream& operator <<(std::ostream &os, const Text &rhs){
		os << "Text: { X: " << rhs.m_x << ", Y: " << rhs.m_y << ", Width: " << rhs.getWidth()
			<< ", Height: " << rhs.getHeight() << " }" << std::endl;
		return os;
	}
};

#endif