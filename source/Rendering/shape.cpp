#include "shape.hpp"


shape::shape()
{
}


shape::~shape()
{
}

point3 shape::getPosition(){
	cout << "unimplemented method: getPosition() called!" <<  endl;
	return point3(0, 0, 0);
}

void shape::getIntersection(size_t subShapeIdx, size_t subShapeIdx2, Ray ray, DifferentialGeometry &closest, float minT, bool bfc){
	cout << "unimplemented method: getIntersection() called!" << endl;
}

bool shape::shadowIntersection(size_t subShapeIdx, size_t subShapeIdx2, line ln){
	cout << "unimplemented method: shadowIntersection() called!" << endl;
	return false;
}

shapeType shape::getType(){
	cout << "unimplemented method: getType() called!" << endl;
	return UNDEFINED;
}

AABB shape::getBoundingBox(size_t subShapeIdx, size_t subShapeIdx2){
	cout << "unimplemented method: getBoundingBox() called!" << endl;
	return AABB();
}

point3 shape::getObjectCenter(size_t subShapeIdx, size_t subShapeIdx2){
	cout << "unimplemented method: getObjectCenter() called!" << endl;
	return point3(0,0,0);
}