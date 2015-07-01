#pragma once
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_TTF.h>

#include "../../LightWhatRenderer/Utility/common.hpp"
struct Font
{
public:
	Font(std::string filepath, int fontSize);
	virtual ~Font();

	TTF_Font *sdlFont;
	int size;
};

