#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "common.hpp"
#include <random>

using namespace std;

struct Ray{
	Ray();
	Ray(line, int, bool);
	inline void precalculate(){
		invDir = vec3(1 / ln.dir.x, 1 / ln.dir.y, 1 / ln.dir.z);
	}
	line ln;
	vec3 invDir;
	int bounces;
	bool isPrimary;
};

struct Camera
{
	Camera();
	Camera(point3, vec3, int, int);
	inline void setTarget(point3 target){
		direction = (target - origin).Norm();
	}

	inline void setParameters(float lAngle, float lNearClipPlane, float lFarClipPlane){
		angle = lAngle, nearClipPlane = lNearClipPlane, farClipPlane = lFarClipPlane;
	}
	void setupImagePlane();
	vector<vector<Ray> > getRayMap(int);

	point3 origin;
	vec3 direction;
	int screenX;
	int screenY;
	float angle;
	float nearClipPlane;
	float farClipPlane;

private:
	point3 planeO;
	vec3 planeX;
	vec3 planeY;
};

struct DifferentialGeometry{
	DifferentialGeometry();
	DifferentialGeometry(intersection, vec3, vec3, point2, size_t, int);
	intersection i;
	vec3 n;
	vec3 t;
	vec3 b;
	vec3 dir;
	point2 uv;
	size_t mat;
	int bounces;
	bool hasTangentSpace = false;
};

struct tile{
	int posX, posY;
	int scaleX, scaleY;
	bool isRendered = false;
};
