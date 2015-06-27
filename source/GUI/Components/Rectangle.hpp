#pragma once
#include "../../Rendering/common.hpp"
struct Rect
{
	Rect(vec2 position, vec2 dimensions, colRGB color);
	Rect();
	vec2 pos, dim;
	colRGB col;
};

