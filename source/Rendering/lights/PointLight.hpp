#pragma once
#include "../light.hpp"

#include "../shapes/Sphere.hpp"
class PointLight : public light
{
public:
	PointLight();
	PointLight(point3 Center, colRGB Color, float Intensity);
	~PointLight();
	LightType getType();
	point3 getPosition();
	float getIntensity();
	colRGB getColor();
	Sphere *solidify(size_t matIdx, colRGB &colRef);

	point3 center;
	colRGB col;
	float intensity = 1;
};

