#pragma once
#include "../../Utility/commonR.hpp"
#include "../shape.hpp"
#include <vector>


class Plane : public shape
{
public:
	Plane();
	Plane(plane, size_t materialIndex);
	virtual ~Plane();


	// C++11 make the class non-copyable
	Plane(const Plane&) = delete;
	Plane& operator=(const Plane&) = delete;

	//shape methods
	void getIntersection(size_t subShapeIdx, size_t subShapeIdx2, Ray ray, DifferentialGeometry &closest, float minT, bool bfc);
	point3 getPosition();
	AABB getBoundingBox(size_t, size_t);
	point3 getObjectCenter(size_t subShapeIdx, size_t subShapeIdx2);
	bool shadowIntersection(size_t subShapeIdx, size_t subShapeIdx2, line ln);
	shapeType getType();
	bool castsShadow();

	//methods
	void setMaterial(size_t materialIndex);

	bool shadowCast = true;
private: 
	plane p;
	size_t m_MatIndex;
};

 
