#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "common.hpp"

using namespace std;

enum shapeType{
	MESH,
	SPHERE,
	PLANE,
	UNDEFINED
};

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

struct Light{
	Light();
	Light(point3, colRGB, double);
	point3 center;
	colRGB col;
	double intensity = 1;
};

struct Camera
{
	Camera();
	Camera(point3, vec3, int, int);
	inline void setTarget(point3 target){
		direction = (target - origin).Norm();
	}

	inline void setParameters(double lAngle, double lNearClipPlane, double lFarClipPlane){
		angle = lAngle, nearClipPlane = lNearClipPlane, farClipPlane = lFarClipPlane;
	}
	void setupImagePlane();
	vector<vector<Ray> > getRayMap(int);

	point3 origin;
	vec3 direction;
	int screenX;
	int screenY;
	double angle;
	double nearClipPlane;
	double farClipPlane;

private:
	point3 planeO;
	vec3 planeX;
	vec3 planeY;
};

enum ShadingModel
{
	DIFFUSE,
	REFLECT,
	PHONG,
	GLOSSY,
	EMISSION,
	BACKGROUND,
	MIX
};

struct PhongParameters
{
	PhongParameters();
	PhongParameters(double ambient, double diffuse, double specular, double exponent);
	double ka;
	double kd;
	double ks;
	double ke;
};

struct Shader
{
	Shader();
	Shader(ShadingModel, colRGB, colRGB, PhongParameters);
	inline string printShader(){
		return to_string(shade) + string(", Spec: ")
			+ to_string(specular.red) + string(", ") + to_string(specular.green) + string(", ") + to_string(specular.blue)
			+ string(", Diff: ") + to_string(diffuse.red) + string(", ") + to_string(diffuse.green) + string(", ") + to_string(diffuse.blue)
			+ string(", param: ") + to_string(param.ka) + string(", ") + to_string(param.kd) + string(", ") + to_string(param.ks) + string(", ") + to_string(param.ke);
	}
	
	ShadingModel shade;

	colRGB diffuse;
	bool hasDifTex = false;
	size_t difTexIdx;

	colRGB specular;
	bool hasSpecTex = false;
	size_t specTexIdx;

	bool hasNormTex = false;
	size_t normTexIdx;

	PhongParameters param;
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