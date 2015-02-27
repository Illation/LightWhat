#include "shape.h"


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

void shape::getIntersection(Ray ray, DifferentialGeometry &closest, double minT, bool bfc){
	cout << "unimplemented method: getIntersection() called!" << endl;
}

bool shape::shadowIntersection(line ln){
	cout << "unimplemented method: shadowIntersection() called!" << endl;
	return false;
}

shapeType shape::getType(){
	cout << "unimplemented method: getType() called!" << endl;
	return UNDEFINED;
}