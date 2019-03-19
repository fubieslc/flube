#include "Vec.hpp"

/* VEC2 */

Vec2 operator+(const Vec2 & a, const Vec2 & b) {
	return Vec2(a.x + b.x, a.y + b.y);
}

Vec2 operator-(const Vec2 & a, const Vec2 & b) {
	return Vec2(a.x - b.x, a.y - b.y);
}

Vec2 operator*(const Vec2 & vec, const int & scalar) {
	return Vec2(vec.x * scalar, vec.y * scalar);
}

Vec2 operator/(const Vec2 & vec, const int & scalar) {
	return Vec2(vec.x / scalar, vec.y / scalar);
}

Vec2 operator*(const Vec2 & vec, const float & scalar) {
	return Vec2((int)(vec.x * scalar), (int)(vec.y * scalar));
}

Vec2 operator/(const Vec2 & vec, const float & scalar) {
	return Vec2((int)(vec.x / scalar), (int)(vec.y / scalar));
}

Vec2 operator-(const Vec2 & a) {
	return Vec2(-a.x, -a.y);
}

float Dist(const Vec2 & vec) {
	return std::sqrtf(vec.x*vec.x + vec.y*vec.y);
}

int Scalar(const Vec2& a, const Vec2& b) {
	return (a.x * b.x) + (a.y * b.y);
}

/* VEC2 END */

/* VEC2_F */

Vec2_F operator+(const Vec2_F & a, const Vec2_F & b) {
	return Vec2_F(a.x + b.x, a.y + b.y);
}

Vec2_F operator-(const Vec2_F & a, const Vec2_F & b) {
	return Vec2_F(a.x - b.x, a.y - b.y);
}

Vec2_F operator*(const Vec2_F & vec, const float & scalar) {
	return Vec2_F(vec.x * scalar, vec.y * scalar);
}

Vec2_F operator/(const Vec2_F & vec, const float & scalar) {
	return Vec2_F(vec.x / scalar, vec.y / scalar);
}

bool operator==(const Vec2 & a, const Vec2 & b) {
	return (a.x == b.x && a.y == b.y);
}

bool operator==(const Vec2_F & a, const Vec2_F & b) {
	return (a.x == b.x && a.y == b.y);
}

Vec2_F operator-(const Vec2_F & a) {
	return Vec2_F(-a.x, -a.y);
}

float Dist(const Vec2_F & vec) {
	return std::sqrt(vec.x*vec.x + vec.y*vec.y);
}

Vec2_F Normalise(const Vec2_F & vec) {
	return vec / Dist(vec);
}

float Scalar(const Vec2_F& a, const Vec2_F& b) {
	return (a.x * b.x) + (a.y * b.y);
}

float Cosine(const Vec2_F & a, const Vec2_F & b) {
	return Scalar(Normalise(a), Normalise(b));
}

float Angle(const Vec2_F & a, const Vec2_F & b) {
	return acos(Cosine(a,b));
}

/* VEC2_F END */

/* Rect */

Rect Abs(const Rect & rect) {
	Rect result = rect;
	if (result.w < 0) {
		result.x += result.w;
		result.w = -result.w;
	}

	if (result.h < 0) {
		result.y += result.h;
		result.h = -result.h;
	}

	return result;
}

Rect operator+(const Rect & rect, const Vec2 & translate) {
	return Rect(rect.x + translate.x, rect.y + translate.y, rect.w, rect.h);
}

Rect operator-(const Rect & rect, const Vec2 & translate) {
	return Rect(rect.x - translate.x, rect.y - translate.y, rect.w, rect.h);
}

Vec2 Centre(const Rect & rect) {
	return Vec2(rect.x + (rect.w>>1), rect.y + (rect.h>>1));
}

Rect Scale(const Rect & rect, const float & scale, const Vec2 & centre) {
	return Rect( centre.x + (int)((rect.x - centre.x) * scale),
	             centre.y + (int)((rect.y - centre.y) * scale),
		     (int)(rect.w * scale), (int)(rect.h * scale));
}

Rect Scale(const Rect & rect, const float & scale) {
	return Scale(rect, scale, Centre(rect));
}

/* Rect_F */

Rect_F Abs(const Rect_F & rect) {
	Rect_F result = rect;
	if (result.w < 0.0f) {
		result.x += result.w;
		result.w = -result.w;
	}

	if (result.h < 0.0f) {
		result.y += result.h;
		result.h = -result.h;
	}

	return result;
}

Rect_F operator+(const Rect_F & rect, const Vec2_F & translate) {
	return Rect_F(rect.x + translate.x, rect.y + translate.y, rect.w, rect.h);
}

Rect_F operator-(const Rect_F & rect, const Vec2_F & translate) {
	return Rect_F(rect.x - translate.x, rect.y - translate.y, rect.w, rect.h);
}

Vec2 Centre(const Rect_F & rect) {
	return Vec2(rect.x + (rect.w/2.0f), rect.y + (rect.h/2.0f));
}

Rect_F Scale(const Rect_F & rect, const float & scale, const Vec2 & centre) {
	return Rect_F( centre.x + (int)((rect.x - centre.x) * scale),
		centre.y + (int)((rect.y - centre.y) * scale),
		(int)(rect.w * scale), (int)(rect.h * scale));
}

Rect_F Scale(const Rect_F & rect, const float & scale) {
	return Scale(rect, scale, Centre(rect));
}

Rect::operator SDL_Rect() {
	return { x,y,w,h };
}

Vec2::operator Vec2_F() {
	return Vec2_F(x,y);
}

Vec2_F::operator Vec2() {
	return Vec2(x,y);
}
