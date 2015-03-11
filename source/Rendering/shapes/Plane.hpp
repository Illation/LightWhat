#pragma once
#include "../commonR.hpp"
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
	void getIntersection(Ray ray, DifferentialGeometry &closest, double minT, bool bfc);
	point3 getPosition();
	AABB getBoundingBox(size_t, size_t);
	point3 getObjectCenter(size_t subShapeIdx, size_t subShapeIdx2);
	bool shadowIntersection(line ln);
	shapeType getType();

	//methods
	void setMaterial(size_t materialIndex);
private: 
	plane p;
	size_t m_MatIndex;
};

 
