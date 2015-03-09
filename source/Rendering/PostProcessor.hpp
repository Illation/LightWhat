#pragma once
#include <vector>
#include "common.hpp"
#include "Texture.hpp"

enum ExposureType{
	AUTO,
	CLIP
};

class PostProcessor
{
public:
	PostProcessor();
	~PostProcessor();

	void controlExposure(Texture &image, int resX, int resY, ExposureType);
	void updateHighestExposure(Texture image, int resX, int reY);
private:
	double m_HighestExposure = 0;
};

