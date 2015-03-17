#pragma once
#include "commonR.hpp"
#include <iostream>

enum LightType{
	LIGHT_POINT,
	LIGHT_AREA,
	LIGHT_UNDEFINED
};

class light
{
public:
	light();
	virtual ~light();
	virtual LightType getType()=0;
	virtual point3 getPosition();
	virtual float getIntensity();
	virtual colRGB getColor();


};
