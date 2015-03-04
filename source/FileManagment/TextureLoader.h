#pragma once
#include <../IL/il.h>
#include <../IL/ilu.h>
#include <../IL/ilut.h>
#include "../Rendering/Texture.h"

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	Texture getTexture(string fileName, bool &success);
};