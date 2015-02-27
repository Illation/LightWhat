#pragma once
#include "../commonR.h"
#include "../shape.h"
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
	bool shadowIntersection(line ln);
	shapeType getType();

	//methods
	void setMaterial(size_t materialIndex);
private: 
	plane p;
	size_t m_MatIndex;
};

 
