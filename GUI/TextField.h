#ifndef _TEXT_FIELD_H_
#define _TEXT_FIELD_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "Texture.h"

class TextField {
public:
	enum FieldType {
		TEXT, NUMBER
	};
private:
	/* unsigned */ int m_x, m_y;
	/* unsigned */ int m_width;
	
	const int m_paddingX = 3, m_paddingY = 5;

	int m_limit = -1;
	unsigned int m_cursor = 0;
	unsigned int m_cursor_ppos = 0;
	void calculateCursorPPos(unsigned int& cursorPPos);

	bool m_selected = false;
	
	TTF_Font *m_font = nullptr;

	std::string m_content = "";
	std::string m_oldContent;
	
	FieldType m_fieldType;
	
	Texture* m_texture = NULL;
	void genTexture(SDL_Renderer *renderer);

public:
	TextField(SDL_Renderer *renderer, const FieldType fieldType, const std::string content, TTF_Font *font, const int x, const int y, const int width, const int limit);
	virtual ~TextField();
	
	const std::string getContent() const {
		if (m_fieldType == NUMBER && m_content.length() == 0)
			return "0";
		return m_content; }
	
	const bool isSelected() { return m_selected; }
	void select() { SDL_StartTextInput(); m_selected = true; }
	void deselect() { SDL_StopTextInput(); m_selected = false; }

	bool m_changed = false;
	
	void renderTextField(SDL_Renderer *renderer, const int& time) const;
	void updateTextField(SDL_Renderer *renderer);
	
	void onKeyPress(const SDL_Keysym& keysym, const std::string& text);
	const bool isInside(const /* unsigned */ int& x, const /* unsigned */ int& y) const;

	const unsigned int getWidth() const;
	const unsigned int getHeight() const;
	const unsigned int getX() const { return m_x; }
	const unsigned int getY() const { return m_y; }
	void setX(const unsigned int x) { m_x = x; }
	void setY(const unsigned int y) { m_y = y; }
	void set(const unsigned int x, const unsigned int y) { m_x = x; m_y = y; }

	friend std::ostream& operator <<(std::ostream &os, const TextField &rhs){
		os << "TextField: { X: " << rhs.m_x << ", Y: " << rhs.m_y << ", Width: " << rhs.getWidth()
			<< ", Height: " << rhs.getHeight() << " }" << std::endl;
		return os;
	}

};

#endif
