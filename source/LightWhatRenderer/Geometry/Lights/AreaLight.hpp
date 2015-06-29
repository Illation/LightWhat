#pragma once
#include "../light.hpp"

#include "../shapes/Mesh.hpp"
class AreaLight : public light
{
public:
	AreaLight();
	AreaLight(vec3 dir, plane pl, float scale, int samples, colRGB Color, float Intensity);
	~AreaLight();
	LightType getType();
	point3 getPosition();
	float getIntensity();
	colRGB getColor();
	Mesh *solidify(size_t matIdx, colRGB &colRef);

	point3 pos, center;
	vec3 dir1, dir2;
	int samplesSq;
	colRGB col;
	float intensity = 1;
};

