#include "Hitreg.hpp"
#include "Game.hpp"

bool RectPointCollision(const Rect & rect, const Vec2 & vec) {
	return (vec.x >= rect.x) && (vec.x <= rect.x + rect.w) &&
	       (vec.y >= rect.y) && (vec.y <= rect.y + rect.h);
}

bool RectPointCollision(const Rect_F & rect, const Vec2_F & vec) {
	return (vec.x >= rect.x) && (vec.x <= rect.x + rect.w) &&
	       (vec.y >= rect.y) && (vec.y <= rect.y + rect.h);
}

bool LineLineCollision(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d) {
	if ((a == c) || (a == d) || (b == c) || (b == d))
		return true;

	float uA, uB;
	uA = ((d.x-c.x)*(a.y-c.y) - (d.y-c.y)*(a.x-c.x)) / (float)((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y));
	uB = ((b.x-a.x)*(a.y-c.y) - (b.y-a.y)*(a.x-c.x)) / (float)((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y)); 

	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
		return true;
	}

	return false;
}

bool LineLineCollision(const Vec2_F & a, const Vec2_F & b, const Vec2_F & c, const Vec2_F & d) {
	if ((a == c) || (a == d) || (b == c) || (b == d))
		return true;

	float uA, uB;
	uA = ((d.x-c.x)*(a.y-c.y) - (d.y-c.y)*(a.x-c.x)) / ((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y));
	uB = ((b.x-a.x)*(a.y-c.y) - (b.y-a.y)*(a.x-c.x)) / ((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y)); 

	if (uA >= 0.0f && uA <= 1.0f && uB >= 0.0f && uB <= 1.0f) {
		return true;
	}

	return false;
}
