#include "commonR.h"

//Rays
Ray::Ray(){
}
Ray::Ray(line lLn, int lBounce, bool lPrimary){
	ln = lLn;
	bounces = lBounce;
	isPrimary = lPrimary;
}


//Light
Light::Light(){
}
Light::Light(point3 lCenter, colRGB lCol, double lInt){
	center = lCenter;
	col = lCol;
	intensity = lInt;
}


//Camera
Camera::Camera(){
}
Camera::Camera(point3 pos, vec3 dir, int Cols, int Rows){
	origin = pos;
	direction = dir;
	pixelCols = Cols;
	pixelRows = Rows;
}
void Camera::setParameters(double lA, double nCP, double fCP){
	angle = lA, nearClipPlane = nCP, farClipPlane = fCP;
}
void Camera::setupImagePlane(){
	center = point3(0, 0, 5);
	double width = 1*2;
	double height = (double)pixelRows / (double)pixelCols*2;
	planeO = point3(-(width/2), (height/2), 4);
	planeX = vec3(width, 0, 0);
	planeY = vec3(0, -height, 0);
}
vector<vector<Ray> > Camera::getRayMap(int maxBounces){
	Camera::setupImagePlane();
	vector<vector<Ray> > ret;
	for (int i = 0; i < pixelCols; i++)
	{
		vector<Ray> rayColumn;
		for (int j = 0; j < pixelRows; j++)
		{
			double x = (double)(i + 1) / pixelCols;
			double y = (double)(j + 1) / pixelRows;
			vec3 pixelPos = planeO + (planeX*x) + (planeY*y);
			rayColumn.push_back(Ray(line(center, pixelPos - center), maxBounces, true));
		}
		ret.push_back(rayColumn);
	}
	return ret;
}


//PhongParameters
PhongParameters::PhongParameters(){
	ka = 0;
	kd = 0;
	ks = 0;
	ke = 0;
}
PhongParameters::PhongParameters(double a, double d, double s, double e){
	ka = a;
	kd = d;
	ks = s;
	ke = e;
}


//Shader
Shader::Shader(){
	shade = PHONG;
	diffuse = colRGB(0, 0, 0);
	specular = colRGB(0, 0, 0);
	param = PhongParameters(0, 0, 0, 0);
}
Shader::Shader(ShadingModel lshade, colRGB ldif, colRGB lspec, PhongParameters lParam){
	shade = lshade;
	diffuse = ldif;
	specular = lspec;
	param = lParam;
}
string Shader::printShader(){
	return to_string(shade) + string(", Spec: ")
		+ to_string(specular.red) + string(", ") + to_string(specular.green) + string(", ") + to_string(specular.blue)
		+ string(", Diff: ") + to_string(diffuse.red) + string(", ") + to_string(diffuse.green) + string(", ") + to_string(diffuse.blue)
		+ string(", param: ") + to_string(param.ka) + string(", ") + to_string(param.kd) + string(", ") + to_string(param.ks) + string(", ") + to_string(param.ke);
}


//Differential geometry
DifferentialGeometry::DifferentialGeometry(){
}
DifferentialGeometry::DifferentialGeometry(intersection lI, vec3 lN, vec3 lDir, point2 lUV, size_t materialIndex, int lBounce){
	i = lI;
	n = lN;
	dir = lDir;
	uv = lUV;
	mat = materialIndex;
	bounces = lBounce;
}