#include "PostProcessor.h"


PostProcessor::PostProcessor()
{
}


PostProcessor::~PostProcessor()
{
}

std::vector<vector<colRGB> >PostProcessor::controlExposure(std::vector<vector<colRGB> > image, int resX, int resY, ExposureType expT){

	double dHighest = 0;
	switch (expT)
	{
	case AUTO:
		if (m_HighestExposure > 1)
		{
			dHighest = 1 / m_HighestExposure;
			for (int i = 0; i < resX; i++)
			{
				for (int j = 0; j < resY; j++)
				{
					colRGB col = image[i][j];
					col *= dHighest;
					image[i][j] = col;
				}
			}
		}
		return image;
		break;
	case CLIP:
		return image;
		break;
	default:
		return image;
		break;
	}
}

void PostProcessor::updateHighestExposure(std::vector<vector<colRGB> > image, int resX, int resY){
	m_HighestExposure = 0;
	for (int i = 0; i < resX; i++)
	{
		for (int j = 0; j < resY; j++)
		{
			colRGB col = image[i][j];
			if (col.red>m_HighestExposure)m_HighestExposure = col.red;
			if (col.green>m_HighestExposure)m_HighestExposure = col.green;
			if (col.blue>m_HighestExposure)m_HighestExposure = col.blue;
		}
	}
}