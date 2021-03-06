#pragma once
#include <vector>
#include "../Utility/common.hpp"
#include "../Utility/Texture.hpp"

enum ExposureType{
	AUTO,
	CLIP
};

class PostProcessor
{
public:
	PostProcessor();
	~PostProcessor();

	void controlExposure(Texture &imageIn, Texture &imageOut, int resX, int resY, ExposureType);
	void updateHighestExposure(Texture &image, int resX, int reY);
private:
	float m_HighestExposure = 0;
};

