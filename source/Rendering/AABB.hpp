#pragma once
#include "commonR.hpp"


class AABB
{
public:
	AABB();
	AABB(point3 p1, point3 p2);
	~AABB();

	bool intersect(Ray ray);

	point3 m_Min, m_Max;
};

