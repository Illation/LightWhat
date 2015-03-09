#pragma once
#include "commonR.hpp"
#include <iostream>
class shape
{
public:
	shape();
	virtual ~shape();
	virtual void getIntersection(Ray ray, DifferentialGeometry &closest, double minT, bool bfc)=0;
	virtual bool shadowIntersection(line ln);
	virtual point3 getPosition();
	virtual shapeType getType();
};

