#pragma once
#include "../../Utility/commonR.hpp"


class AABB
{
public:
	AABB();
	AABB(point3 p1, point3 p2);
	~AABB();

	bool intersect(Ray ray);
	bool intersect(Ray ray, float t);
	point3 center();
	float volume();

	point3 m_Min, m_Max;
};

