#include "PointLight.hpp"


PointLight::PointLight()
{
}

PointLight::PointLight(point3 lCenter, colRGB lColor, float lIntensity)
{
	center = lCenter;
	col = lColor;
	intensity = lIntensity;
}

PointLight::~PointLight()
{
}

LightType PointLight::getType(){
	return LightType::LIGHT_POINT;
}

point3 PointLight::getPosition()
{
	return center;
}
float PointLight::getIntensity()
{
	return intensity;
}
colRGB PointLight::getColor()
{
	return col;
}
Sphere *PointLight::solidify(size_t matIdx, colRGB &colRef)
{
	colRef = col*intensity;
	Sphere *spPtr = new Sphere(center, intensity*0.02f, matIdx);
	spPtr->shadowCast = false;
	return spPtr;
}