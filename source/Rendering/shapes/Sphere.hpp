#pragma once
#include "../commonR.hpp"
#include "../shape.hpp"
#include <vector>


class Sphere : public shape
{
public:
	Sphere();
	Sphere(point3 lC, double lR, size_t matIndex);
	virtual ~Sphere();


	// C++11 make the class non-copyable
	Sphere(const Sphere&) = delete;
	Sphere& operator=(const Sphere&) = delete;

	//shape methods
	void getIntersection(Ray ray, DifferentialGeometry &closest, double minT, bool bfc);
	point3 getPosition();
	bool shadowIntersection(line ln);
	shapeType getType();

	//methods
	void setPosition(point3 pos);
	void setRadius(double lR);
	void setMaterial(size_t matIndex);
private: 
	point3 m_Center;
	double m_Radius, m_SqRadius, m_RRadius;
	size_t m_MatIndex;
};

 
