#pragma once
#include "commonR.hpp"
#include "AABB.hpp"
#include <iostream>

enum shapeType{
	MESH,
	SPHERE,
	PLANE,
	UNDEFINED
};

class shape
{
public:
	shape();
	virtual ~shape();
	virtual void getIntersection(size_t subShapeIdx, size_t subShapeIdx2, Ray ray, DifferentialGeometry &closest, float minT, bool bfc) = 0;
	virtual bool shadowIntersection(size_t subShapeIdx, size_t subShapeIdx2, line ln);
	virtual AABB getBoundingBox(size_t subShapeIdx, size_t subShapeIdx2);
	virtual point3 getObjectCenter(size_t subShapeIdx, size_t subShapeIdx2);
	virtual point3 getPosition();
	virtual shapeType getType();
	virtual bool castsShadow();
};

