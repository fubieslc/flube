#pragma once

#include "SDL.h"

#include <cmath>

struct Vec2_F;
struct Vec2 {
	Vec2() : x(0), y(0) { ; }
	Vec2(int _x, int _y) : x(_x), y(_y) { ; }

	int x, y;

	operator Vec2_F();
};

struct Vec2_F {
	Vec2_F() : x(0.0f), y(0.0f) { ; }
	Vec2_F(float _x, float _y) : x(_x), y(_y) { ; }

	float x, y;

	operator Vec2();
};

Vec2 operator+(const Vec2& a, const Vec2& b);
Vec2 operator-(const Vec2& a, const Vec2& b);
Vec2 operator*(const Vec2& vec, const int& scalar);
Vec2 operator/(const Vec2& vec, const int& scalar);
Vec2 operator*(const Vec2& vec, const float& scalar);
Vec2 operator/(const Vec2& vec, const float& scalar);

Vec2_F operator+(const Vec2_F& a, const Vec2_F& b);
Vec2_F operator-(const Vec2_F& a, const Vec2_F& b);
Vec2_F operator*(const Vec2_F& vec, const float& scalar);
Vec2_F operator/(const Vec2_F& vec, const float& scalar);
bool operator==(const Vec2_F& a, const Vec2_F& b);
bool operator==(const Vec2& a, const Vec2& b);

// Negate
Vec2 operator-(const Vec2& a);

float Dist(const Vec2& vec);

// Negate
Vec2_F operator-(const Vec2_F& a);

float Dist(const Vec2_F& vec);
Vec2_F  Normalise(const Vec2_F& vec);
float Scalar(const Vec2_F& a, const Vec2_F& b);

// returns cos(theta) where theta is the angle between vectors a and b
float Cosine(const Vec2_F& a, const Vec2_F& b);
// returns the angle between vectors a and b
float Angle(const Vec2_F& a, const Vec2_F& b);

struct Rect {
	Rect() : x(0), y(0), w(0), h(0) { ; }
	Rect(int _x, int _y, int _w, int _h) :
		x(_x), y(_y), w(_w), h(_h) { ; }

	int x, y, w, h;

	operator SDL_Rect();
};

struct Rect_F {
	Rect_F() : x(0), y(0), w(0), h(0) { ; }
	Rect_F(float _x, float _y, float _w, float _h) :
		x(_x), y(_y), w(_w), h(_h) { ; }

	float x, y, w, h;
};

// makes width & height positive
Rect Abs(const Rect& rect);
Rect operator+(const Rect& rect, const Vec2& translate);
Rect operator-(const Rect& rect, const Vec2& translate);

Vec2 Centre(const Rect&);
Rect Scale(const Rect& rect, const float& scale, const Vec2& centre);
Rect Scale(const Rect& rect, const float& scale); // scales from centre

Rect_F Abs(const Rect_F& rect);
Rect_F operator+(const Rect_F& rect, const Vec2_F& translate);
Rect_F operator-(const Rect_F& rect, const Vec2_F& translate);

Vec2 Centre(const Rect_F&);
Rect_F Scale(const Rect_F& rect, const float& scale, const Vec2& centre);
Rect_F Scale(const Rect_F& rect, const float& scale); // scales from centre