#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

class Texture {
private:
	SDL_Texture* m_texture = NULL;
	unsigned int m_width, m_height;
public:
	Texture(const std::string path);
	Texture(const std::string text, SDL_Color textColor);
	Texture(const std::string text, TTF_Font* font, SDL_Color textColor);
	virtual ~Texture();
	void freeTexture();
	const unsigned int getWidth() const { return m_width; }
	const unsigned int getHeight() const { return m_height; }
	void getSize(unsigned int& width, unsigned int& height) const { width = getWidth(); height = getHeight(); };
	void renderTexture() const;
	void renderTexture(const int x, const int y) const { renderTexture(x, y, m_width, m_height); }
	void renderTexture(const int x, const int y, const int width, const int height) const;
	void renderTexture(const int sx, const int sy, const int swidth, const int sheight, const int dx, const int dy, const int dwidth, const int dheight) const;
	friend std::ostream& operator <<(std::ostream &os, const Texture &rhs){
		os << "Texture: { Width: " << rhs.m_width << ", Height: " << rhs.m_height << " }" << std::endl;
		return os;
	}
};

#endif
