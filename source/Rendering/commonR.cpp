#include "commonR.hpp"

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
	direction = dir.Norm();
	screenX = Cols;
	screenY = Rows;
}
void Camera::setupImagePlane(){
	//get linear screen plane info
	double angleRad = (angle / 180)*PI;
	double aspectRatio = (double)screenX / (double)screenY;
	double halfWidth = tan(angleRad / 2)*nearClipPlane;
	double halfHeight = halfWidth/aspectRatio;
	double width = halfWidth * 2;
	double height = halfHeight * 2;
	//setup camera cross
	vec3 up = vec3(0, 1, 0);
	vec3 right = direction.Cross(up).Norm();
	vec3 camUp = right.Cross(direction).Norm();
	point3 planeCenter = origin + nearClipPlane*direction;
	//calculate screen plane from information
	planeO = planeCenter-right*halfWidth+camUp*halfHeight;
	planeX = right*width;
	planeY = -camUp*height;
}
vector<vector<Ray> > Camera::getRayMap(int maxBounces){
	Camera::setupImagePlane();
	vector<vector<Ray> > ret;
	for (int i = 0; i < screenX; i++)
	{
		vector<Ray> rayColumn;
		for (int j = 0; j < screenY; j++)
		{
			double x = (double)(i + 1) / screenX;
			double y = (double)(j + 1) / screenY;
			vec3 pixelPos = planeO + (planeX*x) + (planeY*y);
			rayColumn.push_back(Ray(line(origin, pixelPos - origin), maxBounces, true));
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