#pragma once
#include <SDL/SDL.h>
#include "../../LightWhatRenderer/Utility/common.hpp"
#include "../../LightWhatRenderer/Utility/Texture.hpp"
class Bitmap
{
public:
	Bitmap(int width, int height);
	Bitmap(Texture);
	virtual ~Bitmap();
	// C++11 make the class non-copyable
	Bitmap(const Bitmap&) = delete;
	Bitmap& operator=(const Bitmap&) = delete;
	//Member Functions
	void SetPixel(int x, int y, colRGB col);
	void SetPixel(int x, int y, colRGB col, float gamma);
	void SetFromTexture(Texture* texPtr);
	void SetFromTexture(Texture* texPtr, float gamma);

	Uint32* GetData();
	int GetWidth();
	int GetHeight();
private:
	Uint32 *m_Pixels = nullptr;
	int m_Width;
	int m_Height;
};

