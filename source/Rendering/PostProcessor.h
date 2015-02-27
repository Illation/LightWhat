#pragma once
#include <vector>
#include "common.h"

enum ExposureType{
	AUTO,
	CLIP
};

class PostProcessor
{
public:
	PostProcessor();
	~PostProcessor();

	std::vector<vector<colRGB> >controlExposure(std::vector<vector<colRGB> > image, int resX, int resY, ExposureType);
	void updateHighestExposure(std::vector<vector<colRGB> > image, int resX, int reY);
private:
	double m_HighestExposure = 0;
};

