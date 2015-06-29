#pragma once
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_TTF.h>

#include "../../LightWhatRenderer/Utility/common.hpp"
struct Font
{
public:
	Font(std::string filepath, int fontSize, colRGB color);
	virtual ~Font();

	void SetColor(colRGB color);

	TTF_Font *sdlFont;
	int size;
	SDL_Color color;
};

