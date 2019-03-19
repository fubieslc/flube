#include "Render.hpp"
#include "SDL2_gfxPrimitives_font.h"

struct Render Render;

void Render::Init() {
	LoadFont();
	LoadTextures();
}

void Render::Quit() {
	if (CHARS != nullptr) {
		for (int i = 0; i < 256; ++i) {
			SDL_DestroyTexture(CHARS[i]);
		}

		delete CHARS;
	}
}

int Render::LoadTextures() {
	std::vector<std::string> files = GetAllFilesInDir(TEXTURES_FOLDER);

	int count = 0;
	for (auto f : files) {
		if (!AddTexture(f)) {
			++count;
			std::cout << "Loaded texture \"" << f << "\"" << std::endl;
		}
	}

	return count;
}

SDL_Texture * Render::GetTexture(const std::string & str) {
	auto it = textures.find(str);
	if (it == textures.end())
		return nullptr;
	else
		return it->second;
}

int Render::AddTexture(const std::string & filename) {
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (surface == nullptr) {
		Log::PrintIMGError();
		return -1;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == nullptr) {
		Log::PrintSDLError();
		return -1;
	}

	std::string str = filename;
	for (auto c = str.begin(); c != str.end(); ++c)
		if (*c == '/')
			*c = '\\';
	textures[str] = texture;
	return 0;
}

void Render::RenderTexture(const std::string & texture, Rect * dest, Rect * src, float angle, SDL_RendererFlip flip, SDL_Color mod) {
	auto t = GetTexture(texture);
	if (t == nullptr) return;
	SDL_SetTextureColorMod(t, mod.r, mod.g, mod.b);
	SDL_SetTextureAlphaMod(t, mod.a);

	SDL_Rect rect, src_rect;
	if (dest) rect = *dest;
	if (src) src_rect = *src;
	SDL_RenderCopyEx(renderer, t, (src == nullptr ? nullptr : &src_rect), (dest == nullptr ? nullptr : &rect),
		angle, nullptr, flip);
}

void Render::Clear(SDL_Color c) {
	SetColor(c);
	SDL_RenderClear(renderer);
}

void Render::SetColor(SDL_Color c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 0xff);
}

void Render::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Render::RenderPixel(const Vec2& vec) {
	SDL_RenderDrawPoint(renderer, vec.x, vec.y);
}
void Render::RenderPixel(const Vec2& vec, SDL_Color c) {
	SetColor(c);
	RenderPixel(vec);
}

void Render::RenderLine(const Vec2& point_a, const Vec2& point_b) {
	SDL_RenderDrawLine(renderer, point_a.x, point_a.y, point_b.x, point_b.y);
}
void Render::RenderLine(const Vec2 & point_a, const Vec2 & point_b, SDL_Color c) {
	SetColor(c);
	RenderLine(point_a, point_b);
}

void Render::RenderVector(const Vec2& vector, const Vec2& offset) {
	RenderLine(offset, offset + vector);
}
void Render::RenderVector(const Vec2& vector, const Vec2& offset, SDL_Color c) {
	SetColor(c);
	RenderLine(offset, offset + vector);
}

void Render::RenderRect(const Rect & rect) {
	SDL_Rect r = { rect.x , rect.y , rect.w , rect.h };
	SDL_RenderDrawRect(renderer, &r);
}
void Render::RenderRect(const Rect & rect, SDL_Color c) { SetColor(c); RenderRect(rect); }

void Render::RenderFillRect(const Rect & rect) {
	SDL_Rect r = { rect.x , rect.y , rect.w , rect.h };
	SDL_RenderFillRect(renderer, &r);
}
void Render::RenderFillRect(const Rect & rect, SDL_Color c) { SetColor(c); RenderFillRect(rect); }


void Render::RenderText(const Vec2 & offset, const std::string & str, int scale, SDL_Color colour) {
	int pos_x = offset.x,
	    pos_y = offset.y;

	Vec2 off = offset;
	SetColor(colour);

	for (unsigned char c : str) {
		if (c == '\n') {
			off.x = offset.x;
			off.y += 8 * scale;
			continue;
		}

		RenderChar(off, c, scale, colour);

		off.x += 8 * scale;
	}
}

void Render::RenderChar(const Vec2& offset, const unsigned char c, int scale, SDL_Color colour) {
	SDL_Rect rect = { offset.x , offset.y , scale * 8, scale * 8 };
	SDL_Texture* char_texture = CHARS[c];
	SDL_SetTextureColorMod(char_texture, colour.r, colour.g, colour.b);

	if (char_texture == nullptr) return;

	SDL_RenderCopy(renderer, char_texture, nullptr, &rect);

	/*const Uint8 mask[8] =
	      { 0b10000000 ,
		0b01000000 ,
		0b00100000 ,
		0b00010000 ,
		0b00001000 ,
		0b00000100 ,
		0b00000010 ,
		0b00000001 };

	unsigned char * data = gfxPrimitivesFontdata + (c * 8);
	for (unsigned char * row = data; row != data + 8; ++row) {
		rect.x = offset.x;

		for (char bit = 0; bit < 8; ++bit) {
			if (mask[bit] & *row) {
				SDL_RenderDrawPoint(renderer, rect.x, rect.y);
			}

			rect.x += rect.w;
		}

		rect.y += rect.h;
	}*/
}

Vec2 Render::GetTextSize(const std::string & str, int scale) {
	Vec2 size = Vec2(0, 1);
	int curr_width = 0;

	for (auto c : str) {
		if (c == '\n') {
			curr_width = 0;
			size.y++;
		}
		else {
			++curr_width;
			if (size.x < curr_width)
				size.x = curr_width;
		}
	}

	// times by character size
	size = size * (8 * scale);
	return size;
}

void Render::UpdateRender() {
	SDL_RenderPresent(renderer);
}

void Render::LoadFont() {
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	const Uint8 mask[8] =
	{ 0b10000000 ,
		0b01000000 ,
		0b00100000 ,
		0b00010000 ,
		0b00001000 ,
		0b00000100 ,
		0b00000010 ,
		0b00000001 };

	CHARS = new SDL_Texture*[256];
	for (int i = 0; i < 256; ++i) {
		SDL_Surface* surface;
		//surface = SDL_CreateRGBSurface(0, 8, 8, 32, rmask, gmask, bmask, amask);
		surface = SDL_CreateRGBSurfaceWithFormat(0, 8, 8, 32, SDL_PIXELFORMAT_RGBA32);
		if (surface == nullptr) return;

		SDL_Rect rect = {0,0,8,8};
		// make fully transparent
		//SDL_FillRect(surface, &rect, 0x00);

		for (int y = 0; y < 8; ++y) {
			for (int x = 0; x < 8; ++x) {
				if (mask[x] & gfxPrimitivesFontdata[(i * 8) + y]) {
					rect = { x,y,1,1 };
					SDL_FillRect(surface, &rect, 0xffffffff);
				}
			}
			std::cout << std::endl;;
		}
	
		CHARS[i] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
}
