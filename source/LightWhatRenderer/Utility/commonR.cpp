#include "commonR.hpp"

//Rays
Ray::Ray(){
}
Ray::Ray(line lLn, int lBounce, bool lPrimary){
	ln = lLn;
	bounces = lBounce;
	isPrimary = lPrimary;
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
	float angleRad = (angle / 180.f)*PI;
	float aspectRatio = (float)screenX / (float)screenY;
	float halfWidth = tan(angleRad / 2)*nearClipPlane;
	float halfHeight = halfWidth/aspectRatio;
	float width = halfWidth * 2;
	float height = halfHeight * 2;
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
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);
	for (int i = 0; i < screenX; i++)
	{
		vector<Ray> rayColumn;
		for (int j = 0; j < screenY; j++)
		{
			float x = (float)(i + dis(gen)) / screenX;
			float y = (float)(j + dis(gen)) / screenY;
			vec3 pixelPos = planeO + (planeX*x) + (planeY*y);
			Ray ray = Ray(line(origin, pixelPos - origin), maxBounces, true);
			ray.precalculate();
			rayColumn.push_back(ray);
		}
		ret.push_back(rayColumn);
	}
	return ret;
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