#pragma once
#include "commonR.hpp"

enum interpolationMode{
	INTPOL_LINEAR,
	INTPOL_PIXELS
};
enum fitMode{
	FIT_STRETCHXY,
	FIT_X,
	FIT_Y
};
class Texture
{
public:
	Texture();
	Texture(string lName, int mX, int mY);
	~Texture();

	void setRGB(colRGB col, int x, int y);
	void setAlpha(float alpha, int x, int y);
	void setName(string lName);
	void setInterpolationMode(interpolationMode lIPM);
	void setQuadraticFittingMode(fitMode lFit);

	colRGB getRGB(int x, int y);
	colRGB getRGB(float x, float y);
	float getAlpha(int x, int y);
	float getAlpha(float x, float y);

	size_t getWidth();
	size_t getHeight();
	string getName();

	bool hasAlpha;
private:
	string m_Name;
	vector<vector<colRGB> > m_RGBcolours;
	vector<vector<float> > m_AlphaValues;
	size_t m_PixelsX;
	size_t m_PixelsY;
	interpolationMode m_IntPolMode;
	fitMode m_FitMode = FIT_STRETCHXY;
};

