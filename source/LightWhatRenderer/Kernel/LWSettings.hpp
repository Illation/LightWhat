#pragma once
#include "PostProcessor.hpp"

struct LWSettings
{
	LWSettings();
	virtual ~LWSettings();

	// C++11 make the class non-copyable
	LWSettings(const LWSettings&) = delete;
	LWSettings& operator=(const LWSettings&) = delete;

	int resolutionX;
	int resolutionY;
	int samples;
	int bounces;
	bool backfaceCulling;
	ExposureType toneMapping;
	bool gammaEnabled;
	float gammaExponent;
	colRGB background;
};