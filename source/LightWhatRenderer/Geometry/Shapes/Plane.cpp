#include "Plane.hpp"

Plane::Plane(){
}
Plane::Plane(plane lP, size_t mIndex){
	p = lP;
	m_MatIndex = mIndex;
}
Plane::~Plane(){
}

void Plane::getIntersection(size_t subShapeIdx, size_t subShapeIdx2, Ray ray, DifferentialGeometry &closest, float minT, bool bfc){
	intersection i;
	i = p.rayIts(ray.ln, bfc);
	if (i.hit){
		if (i.t > minT && (i.t < closest.i.t || closest.i.hit == false))
		{
			closest.i.hit = true;
			closest.i.t = i.t;
			closest.i.backfacing = i.backfacing;
			closest.i.p = ray.ln.orig + ray.ln.dir*i.t;
			closest.n = p.n;
			closest.mat = m_MatIndex;
			closest.hasTangentSpace = false;
		}
	}
}
bool Plane::shadowIntersection(size_t subShapeIdx, size_t subShapeIdx2, line ln){
	intersection its = p.lineIts(ln);
	bool ret = false;
	float shadowLength = ln.dir.Length();
	if (its.hit)
	{
		if (its.t>0.0000000001f && its.t < shadowLength)ret = true;
	}
	return ret;
}
AABB Plane::getBoundingBox(size_t i1, size_t i2){
	return AABB(point3(-0.f, -0.f, -0.f), point3(0.f, 0.f, 0.f));
}
point3 Plane::getObjectCenter(size_t subShapeIdx, size_t subShapeIdx2){
	return p.n*p.d;
}
point3 Plane::getPosition(){
	float Distance = p.d / p.n.Length();
	point3 origin = p.n.Norm(0.000001f)*Distance;
	return origin;
}

void Plane::setMaterial(size_t mIndex){
	m_MatIndex = mIndex;
}

shapeType Plane::getType(){
	return PLANE;
}


bool Plane::castsShadow()
{
	return shadowCast;
}