#include "AABB.hpp"


AABB::AABB()
{
}
AABB::AABB(point3 p1, point3 p2){
	m_Min = point3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
	m_Max = point3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
}

AABB::~AABB()
{
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