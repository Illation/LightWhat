#include "Font.hpp"


Font::Font(std::string filepath, int fontSize)
{
	sdlFont = TTF_OpenFont(filepath.c_str(), fontSize);
	size = fontSize;
}

Font::~Font()
{
	TTF_CloseFont(sdlFont);
}