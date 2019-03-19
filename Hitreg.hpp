#pragma once

#include <algorithm>
#include <iostream>

#include "Vec.hpp"

bool RectPointCollision(const Rect& rect, const Vec2& vec);
bool RectPointCollision(const Rect_F& rect, const Vec2_F& vec);

bool LineLineCollision(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d);
bool LineLineCollision(const Vec2_F & a, const Vec2_F & b, const Vec2_F & c, const Vec2_F & d);