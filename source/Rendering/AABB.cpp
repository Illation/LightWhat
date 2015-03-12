#include "AABB.hpp"


AABB::AABB()
{
}
AABB::AABB(point3 p1, point3 p2){
	m_Min = point3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
	m_Max = point3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
	vec3 delta = m_Max - m_Min;
	if (delta.x == 0)m_Max.x += 0.0001;
	if (delta.y == 0)m_Max.y += 0.0001;
	if (delta.z == 0)m_Max.z += 0.0001;
}

AABB::~AABB()
{
}

point3 AABB::center(){
	return m_Min + (m_Max - m_Min) / 2;
}
double AABB::volume(){
	return (m_Max.x - m_Min.x)*(m_Max.y - m_Min.y)*(m_Max.z - m_Min.z);
}
bool AABB::intersect(Ray ray){
	double tx1 = (m_Min.x - ray.ln.orig.x)*ray.invDir.x;
	double tx2 = (m_Max.x - ray.ln.orig.x)*ray.invDir.x;

	double tmin = std::min(tx1, tx2);
	double tmax = std::max(tx1, tx2);

	double ty1 = (m_Min.y - ray.ln.orig.y)*ray.invDir.y;
	double ty2 = (m_Max.y - ray.ln.orig.y)*ray.invDir.y;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	double tz1 = (m_Min.z - ray.ln.orig.z)*ray.invDir.z;
	double tz2 = (m_Max.z - ray.ln.orig.z)*ray.invDir.z;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));

	return tmax >= std::max(0.0, tmin);
}
bool AABB::intersect(Ray ray, double t){
	double tx1 = (m_Min.x - ray.ln.orig.x)*ray.invDir.x;
	double tx2 = (m_Max.x - ray.ln.orig.x)*ray.invDir.x;

	double tmin = std::min(tx1, tx2);
	double tmax = std::max(tx1, tx2);

	double ty1 = (m_Min.y - ray.ln.orig.y)*ray.invDir.y;
	double ty2 = (m_Max.y - ray.ln.orig.y)*ray.invDir.y;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	double tz1 = (m_Min.z - ray.ln.orig.z)*ray.invDir.z;
	double tz2 = (m_Max.z - ray.ln.orig.z)*ray.invDir.z;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));

	return tmax >= std::max(0.0, tmin) && tmin <= t;
}