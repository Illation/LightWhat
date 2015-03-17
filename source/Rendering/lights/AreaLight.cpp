#include "AreaLight.hpp"


AreaLight::AreaLight()
{
}
AreaLight::AreaLight(vec3 dir, plane pl, float scale, int samples, colRGB Color, float Intensity)
{
	dir1 = pl.n.Cross(dir).Norm()*scale;
	dir2 = pl.n.Cross(dir1).Norm()*scale;
	center = pl.n*pl.d;
	pos = center - (dir1*0.5) - (dir2*0.5);
	samplesSq = samples;
	col = Color;
	intensity = Intensity;
}

LightType AreaLight::getType(){
	return LightType::LIGHT_AREA;
}

point3 AreaLight::getPosition()
{
	return center;
}
float AreaLight::getIntensity()
{
	return intensity;
}
colRGB AreaLight::getColor()
{
	return col;
}
AreaLight::~AreaLight()
{
}


Mesh *AreaLight::solidify(size_t matIdx, colRGB &colRef)
{
	colRef = col*intensity;
	Mesh *mPtr = new Mesh();
	mPtr->shadowCast = false;
	mPtr->addVertex(pos);
	mPtr->addVertex(pos+dir1);
	mPtr->addVertex(pos+dir2);
	mPtr->addVertex(pos+dir1+dir2);
	mPtr->addPolyList(matIdx, false);
	vec3 N = dir1.Cross(dir2);
	mPtr->addNormal(N);
	mPtr->addNormal(-N);
	mPtr->createTri(0, 1, 2, 0, 0, 0, 0);
	mPtr->createTri(1, 3, 2, 1, 1, 1, 0);
	mPtr->hasTangentSpace = false;
	return mPtr;
}