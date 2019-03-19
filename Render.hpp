#pragma once

#include <string>
#include <thread>
#include <SDL.h>
#include <iostream>
#include <map>

#include "Log.hpp"
#include "Filesystem.hpp"
#include "Vec.hpp"

#ifdef _WIN32
#define TEXTURES_FOLDER "img\\"
#elif defined __linux__
#define TEXTURES_FOLDER "img/"
#endif

// Inheritted by Core
extern struct Render {
	SDL_Renderer* renderer;

	void Init();
	void Quit();

	std::map<std::string, SDL_Texture*> textures;
	int LoadTextures(); // returns how many textures loaded
	SDL_Texture* GetTexture(const std::string& str);
	int AddTexture(const std::string& filename); // -1 for error, 0 for success
	void RenderTexture(const std::string& texture, Rect* dest, Rect* src,
		float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Color mod = {0xff,0xff,0xff,0xff});

	void Clear(SDL_Color c = { 0x0 , 0x0 , 0x0 , 0xff } );
	void SetColor(SDL_Color);
	void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = (Uint8)0xff);

	void RenderPixel(const Vec2& vec);
	void RenderPixel(const Vec2& vec, SDL_Color c);

	void RenderLine(const Vec2& point_a, const Vec2& point_b);
	void RenderLine(const Vec2& point_a, const Vec2& point_b, SDL_Color c);

	void RenderVector(const Vec2& vector, const Vec2& offset);
	void RenderVector(const Vec2& vector, const Vec2& offset, SDL_Color c);

	void RenderRect(const Rect& rect);
	void RenderRect(const Rect& rect, SDL_Color c);

	void RenderFillRect(const Rect& rect);
	void RenderFillRect(const Rect& rect, SDL_Color c);

	void RenderText(const Vec2& offset, const std::string& str, int scale = 1, SDL_Color colour = {0xff,0xff,0xff,0xff});
	void RenderChar(const Vec2& offset, const unsigned char c, int scale = 1, SDL_Color colour = {0xff,0xff,0xff,0xff});
	Vec2 GetTextSize(const std::string& str, int scale = 1);

	void UpdateRender();
private:
	void LoadFont();
	SDL_Texture ** CHARS = nullptr;
} Render;