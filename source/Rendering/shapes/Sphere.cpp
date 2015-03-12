#include "Sphere.hpp"

Sphere::Sphere(){
}
Sphere::Sphere(point3 lC, double lR, size_t matIndex){
	m_Center = lC;
	m_Radius = lR;
	m_SqRadius = lR*lR;
	m_RRadius = 1.0f / lR;
	m_MatIndex = matIndex;
}
Sphere::~Sphere(){
}

void Sphere::getIntersection(size_t subShapeIdx, size_t subShapeIdx2, Ray ray, DifferentialGeometry &closest, double minT, bool bfc){
	vec3 v = ray.ln.orig - m_Center;
	double b = -(v.Dot(ray.ln.dir.Norm(0.0000000001)));
	double det = (b*b) - (v.Dot(v)) + m_SqRadius;
	if (det > 0){
		det = sqrt(det);
		double t = b - det;
		if ((t < closest.i.t || closest.i.hit == false) && t > minT)
		{
			closest.i.hit = true;
			closest.i.t = t;
			closest.i.p = ray.ln.orig + ray.ln.dir*t;
			closest.mat = m_MatIndex;
			closest.n = (closest.i.p - m_Center).Norm(0.0000000001);
		}
	}
}
bool Sphere::shadowIntersection(size_t subShapeIdx, size_t subShapeIdx2, line ln){
	bool ret = false;
	double shadowLength = ln.dir.Length();
	vec3 v = ln.orig - m_Center;
	double b = -(v.Dot(ln.dir.Norm(0.0000000001)));
	double det = (b*b) - (v.Dot(v)) + m_SqRadius;
	if (det > 0){
		det = sqrt(det);
		double t = b - det;
		if (t>0.0000000001 && t < shadowLength)ret = true;
	}
	return ret;
}
point3 Sphere::getPosition(){
	return m_Center;
}
point3 Sphere::getObjectCenter(size_t subShapeIdx, size_t subShapeIdx2){
	return m_Center;
}

AABB Sphere::getBoundingBox(size_t i1, size_t i2){
	point3 min = m_Center - vec3(m_Radius, m_Radius, m_Radius);
	point3 max = m_Center + vec3(m_Radius, m_Radius, m_Radius);
	return AABB(min, max);
}
shapeType Sphere::getType(){
	return SPHERE;
}

void Sphere::setMaterial(size_t matIndex){
	m_MatIndex = matIndex;
}
void Sphere::setRadius(double lR){
	m_Radius = lR;
	m_SqRadius = lR*lR;
	m_RRadius = 1.0f / lR;
}
void Sphere::setPosition(point3 pos){
	m_Center = pos;
}