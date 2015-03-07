#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "common.h"

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
	line ln;
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
	void setTarget(point3);

	void setParameters(double lAngle, double lNearClipPlane, double lFarClipPlane);
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
	string printShader();
	
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