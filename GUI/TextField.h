#ifndef _TEXT_FIELD_H_
#define _TEXT_FIELD_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include <string>

extern SDL_Renderer* g_renderer;
extern TTF_Font* g_font;
extern const int PADDING_X, PADDING_Y;
extern int g_textHeight;
extern bool is_number(const std::string& s);

class TextField {
public:
	enum FieldType {
		TEXT, NUMBER
	};
private:
	/* unsigned */ int m_x, m_y;
	/* unsigned */ int m_width;
	
	int m_limit = -1;
	unsigned int m_cursor = 0;
	unsigned int m_cursor_ppos = 0;
	void calculateCursorPPos(unsigned int& cursorPPos);

	bool m_selected = false;
	
	std::string m_content = "";
	std::string m_oldContent;
	
	FieldType m_fieldType;
	
	Texture* m_texture = NULL;
	void genTexture();

public:
	TextField(const FieldType fieldType, const std::string content, const int x, const int y, const int width, const int limit);
	virtual ~TextField();
	
	const std::string getContent() const { return m_content; }
	
	const bool isSelected() { return m_selected; }
	void select() { SDL_StartTextInput(); m_selected = true; }
	void deselect() { SDL_StopTextInput(); m_selected = false; }
	
	void renderTextField(const int& time) const;
	
	void onKeyPress(const SDL_Keysym& keysym, const std::string& text);
	const bool isInside(const /* unsigned */ int& x, const /* unsigned */ int& y) const;

	void getSize(unsigned int& width, unsigned int& height) const { width = getWidth(); height = getHeight(); };
	const unsigned int getWidth() const;
	const unsigned int getHeight() const;
	const unsigned int getX() const { return m_x; }
	const unsigned int getY() const { return m_y; }

	friend std::ostream& operator <<(std::ostream &os, const TextField &rhs){
		os << "TextField: { X: " << rhs.m_x << ", Y: " << rhs.m_y << ", Width: " << rhs.getWidth()
			<< ", Height: " << rhs.getHeight() << " }" << std::endl;
		return os;
	}

};

#endif
