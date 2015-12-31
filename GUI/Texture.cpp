#include "Texture.h"
#include "../NTTTManager.h"

Texture::Texture(const std::string path){
	
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL){
		std::cout << "Failed at loading image (" << path << "): " << IMG_GetError() << std::endl;
		return;
	}

	m_width = loadedSurface->w;
	m_height = loadedSurface->h;
	
	m_texture = SDL_CreateTextureFromSurface(g_NtttManager.g_renderer, loadedSurface);
	if (m_texture == NULL){
		std::cout << "Failed creating texture from surface: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(loadedSurface);
}

Texture::Texture(const std::string text, const SDL_Color textColor){
	
	SDL_Surface* textSurface = TTF_RenderText_Solid(g_NtttManager.g_font, text.c_str(), textColor);
	if (textSurface == NULL){
		std::cout << "Failed creating Text Surface: " << TTF_GetError() << std::endl;
		return;
	}

	m_width = textSurface->w;
	m_height = textSurface->h;

	m_texture = SDL_CreateTextureFromSurface(g_NtttManager.g_renderer, textSurface);
	if (m_texture == NULL){
		std::cout << "Failed creating texture from surface: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(textSurface);
}

Texture::Texture(const std::string text, TTF_Font* font, const SDL_Color textColor){
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if (textSurface == NULL){
		std::cout << "Failed creating Text Surface: " << TTF_GetError() << std::endl;
		return;
	}

	m_width = textSurface->w;
	m_height = textSurface->h;

	m_texture = SDL_CreateTextureFromSurface(g_NtttManager.g_renderer, textSurface);
	if (m_texture == NULL){
		std::cout << "Failed creating texture from surface: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(textSurface);
}

Texture::~Texture(){
	freeTexture();
}

void Texture::freeTexture() {
	SDL_DestroyTexture(m_texture);
	m_texture = NULL;
}

void Texture::renderTexture() const {
	SDL_RenderCopy(g_NtttManager.g_renderer, m_texture, NULL, NULL);
}

void Texture::renderTexture(const int x, const int y, const int width, const int height) const {
	SDL_Rect dstRect = {x, y, width, height};
	SDL_RenderCopy(g_NtttManager.g_renderer, m_texture, NULL, &dstRect);
}

void Texture::renderTexture(const int sx, const int sy, const int swidth, const int sheight, const int dx, const int dy, const int dwidth, const int dheight) const{
	SDL_Rect srcRect = { sx, sy, swidth, sheight };
	SDL_Rect dstRect = { dx, dy, dwidth, dheight };
	SDL_RenderCopy(g_NtttManager.g_renderer, m_texture, &srcRect, &dstRect);
}
