#include "Font.hpp"


Font::Font(std::string filepath, int fontSize, colRGB c)
{
	sdlFont = TTF_OpenFont(filepath.c_str(), fontSize);
	size = fontSize;
	SetColor(c);
}

Font::~Font()
{
	TTF_CloseFont(sdlFont);
}

void Font::SetColor(colRGB c)
{
	color = { (int)(c.red * 255), (int)(c.green * 255), (int)(c.blue * 255), 255 };
}