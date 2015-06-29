#include "PostProcessor.hpp"


PostProcessor::PostProcessor()
{
}


PostProcessor::~PostProcessor()
{
}

void PostProcessor::controlExposure(Texture &imageIn, Texture &imageOut, int resX, int resY, ExposureType expT){

	float dHighest = 0;
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
					colRGB col = imageIn.getRGB(i, j);
					col *= dHighest;
					imageOut.setRGB(col, i, j);
				}
			}
		}
		break;
	case CLIP:
		for (int i = 0; i < resX; i++)
		{
			for (int j = 0; j < resY; j++)
			{
				colRGB col = imageIn.getRGB(i, j);
				if (col.red>1.f)col.red = 1.f;
				if (col.green>1.f)col.green = 1.f;
				if (col.blue>1.f)col.blue = 1.f;
				imageOut.setRGB(col, i, j);
			}
		}
		break;
	default:
		break;
	}
}

void PostProcessor::updateHighestExposure(Texture &image, int resX, int resY){
	m_HighestExposure = 0;
	for (int i = 0; i < resX; i++)
	{
		for (int j = 0; j < resY; j++)
		{
			colRGB col = image.getRGB(i, j);
			if (col.red>m_HighestExposure)m_HighestExposure = col.red;
			if (col.green>m_HighestExposure)m_HighestExposure = col.green;
			if (col.blue>m_HighestExposure)m_HighestExposure = col.blue;
		}
	}
}