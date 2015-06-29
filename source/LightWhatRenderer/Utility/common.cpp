#include "common.hpp"
using namespace std;
//3D Vector
vec3::vec3(){
}
vec3::vec3(float lx, float ly, float lz){
	x = lx;
	y = ly;
	z = lz;
}

//2D Vector
vec2::vec2(){
}
vec2::vec2(float lx, float ly){
	x = lx;
	y = ly;
}

//Line
line::line(){
}
line::line(point3 lOrigin, vec3 lDir){
	orig = lOrigin;
	dir = lDir;
}


//Plane
plane::plane(){
}
plane::plane(vec3 lN, float lD){
	n = lN;
	d = lD;
}
plane::plane(point3 A, point3 B, point3 C){
	n = (B - A).Cross(C - A).Norm(0.001f);
	d = n.Dot(A);
}


//Intersection
intersection::intersection(){
}
intersection::intersection(bool lHit, point3 lP, float lT){
	hit = lHit;
	p = lP;
	t = lT;
}


//Color
colRGB::colRGB(){
}
colRGB::colRGB(float lx, float ly, float lz){
	red = lx;
	green = ly;
	blue = lz;
}