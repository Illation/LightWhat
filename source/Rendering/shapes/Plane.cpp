#include "Plane.h"

Plane::Plane(){
}
Plane::Plane(plane lP, size_t mIndex){
	p = lP;
	m_MatIndex = mIndex;
}
Plane::~Plane(){
}

void Plane::getIntersection(Ray ray, DifferentialGeometry &closest, double minT, bool bfc){
	intersection i;
	i = p.rayIts(ray.ln, bfc);
	if (i.hit){
		if (i.t > minT && (i.t < closest.i.t || closest.i.hit == false))
		{
			closest.i.hit = true;
			closest.i.t = i.t;
			closest.i.p = ray.ln.orig + ray.ln.dir*i.t;
			closest.n = p.n;
			closest.mat = m_MatIndex;
		}
	}
}
bool Plane::shadowIntersection(line ln){
	intersection its = p.lineIts(ln);
	bool ret = false;
	double shadowLength = ln.dir.Length();
	if (its.hit)
	{
		if (its.t>0.0000000001 && its.t < shadowLength)ret = true;
	}
	return ret;
}
point3 Plane::getPosition(){
	double Distance = p.d / p.n.Length();
	point3 origin = p.n.Norm(0.000001)*Distance;
	return origin;
}

void Plane::setMaterial(size_t mIndex){
	m_MatIndex = mIndex;
}

shapeType Plane::getType(){
	return PLANE;
}