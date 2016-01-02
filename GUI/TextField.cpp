#include "TextField.h"
#include "../NTTTManager.h"

TextField::TextField(SDL_Renderer *renderer, const FieldType fieldType, const std::string content, const int x, const int y, const int width, const int limit) {
	m_fieldType = fieldType;
	m_x = x;
	m_y = y;
	m_width = width;
	m_limit = limit;
	m_content = content;
	genTexture(renderer);
}

TextField::~TextField(){
	delete m_texture;
	m_texture = nullptr;
}

void TextField::genTexture(SDL_Renderer *renderer){
	delete m_texture;
	if (m_content.length() <= 0)
		m_texture = new Texture(renderer, " ", { 0, 0, 0 });
	else
		m_texture = new Texture(renderer, m_content, { 0, 0, 0 });
	m_changed = false;
}

void TextField::updateTextField(SDL_Renderer *renderer){
	if (m_changed)
		genTexture(renderer);
}

void TextField::renderTextField(SDL_Renderer *renderer, const int& time) const {

	SDL_Rect rect = { m_x, m_y, m_width, g_NtttManager.g_textHeight + g_NtttManager.PADDING_Y * 2 };

	if (m_selected)
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	else
		SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);

	SDL_RenderFillRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_RenderDrawRect(renderer, &rect);

	if (m_content.length() <= 0){
		if (m_selected && time % 2000 >= 1000) {
			SDL_RenderDrawLine(renderer, m_x + g_NtttManager.PADDING_X, m_y + g_NtttManager.PADDING_Y, m_x + g_NtttManager.PADDING_X, m_y + g_NtttManager.g_textHeight + g_NtttManager.PADDING_Y);
		}
	}
	else{
		m_texture->renderTexture(renderer, m_x + g_NtttManager.PADDING_X, m_y + g_NtttManager.PADDING_Y);
		if (m_selected && time % 2000 >= 1000) {
			SDL_RenderDrawLine(renderer, m_x + g_NtttManager.PADDING_X + m_cursor_ppos, m_y + g_NtttManager.g_textHeight + g_NtttManager.PADDING_Y,
				m_x + g_NtttManager.PADDING_X + m_cursor_ppos, m_y + g_NtttManager.PADDING_Y);
		}
	}

}

const bool TextField::isInside(const /* unsigned */ int& x, const /* unsigned */ int& y) const{
	return m_x <= x && m_x + m_width >= x && m_y <= y && m_y + g_NtttManager.g_textHeight + g_NtttManager.PADDING_X * 2 >= y;
}

bool is_number(const std::string& s) //Checks if a string is a number
{
	return !s.empty() && s.find_first_not_of("0123456789") == std::string::npos;
}

void TextField::onKeyPress(const SDL_Keysym& keysym, const std::string& text){
	if (!m_selected)
		return;

	if (keysym.sym == SDLK_RETURN) {
		deselect();
		return;
	}

	if (keysym.sym == SDLK_BACKSPACE){
		if (m_cursor > 0){
			m_content.erase(--m_cursor, 1);
			calculateCursorPPos(m_cursor_ppos);
			m_changed = true;
		}
	}

	if (keysym.sym == SDLK_DELETE)
		if (m_cursor < m_content.length()){
			m_content.erase(m_cursor, 1);
			m_changed = true;
		}

	if (keysym.sym == SDLK_LEFT)
		if (m_cursor > 0){
			m_cursor--;
			calculateCursorPPos(m_cursor_ppos);
		}

	if (keysym.sym == SDLK_RIGHT)
		if (m_cursor < m_content.length()){
			m_cursor++;
			calculateCursorPPos(m_cursor_ppos);
		}

	if (m_fieldType == NUMBER){
		if (!is_number(text))
			return;
		m_content.insert(m_cursor, text);
		if (m_limit >= 0 && std::stoll(m_content) > m_limit){
			m_content.erase(m_cursor, 1);
		}
		else{
			m_cursor += text.length();
			calculateCursorPPos(m_cursor_ppos);
			m_changed = true;
		}
	}
	else{
		if (m_limit < 0 || !(m_content.length() + text.length() <= (unsigned) m_limit)){
			m_content.insert(m_cursor, text);
			m_cursor += text.length();
			calculateCursorPPos(m_cursor_ppos);
			m_changed = true;
		}
	}
}



void TextField::calculateCursorPPos(unsigned int& cursorPPos){
	if (m_cursor == 0){
		cursorPPos = 0;
		return;
	}
	int w, h;
	std::string content = m_content;
	content.resize(m_cursor);
	TTF_SizeText(g_NtttManager.g_font, content.c_str(), &w, &h);
	cursorPPos = w;
}

const unsigned int TextField::getWidth() const{
	return m_width;
}

const unsigned int TextField::getHeight() const{
	return g_NtttManager.g_textHeight + g_NtttManager.PADDING_Y * 2;
}
