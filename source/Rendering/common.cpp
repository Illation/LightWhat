#include "common.h"

//3D Vector
vec3::vec3(){
}
vec3::vec3(double lx, double ly, double lz){
	x = lx;
	y = ly;
	z = lz;
}
//methods
double vec3::Dot(const vec3& otherRef) const{
	return x * otherRef.x + y * otherRef.y + z*otherRef.z;
}				
vec3 vec3::Cross(const vec3& otherRef) const{
	return vec3(y * otherRef.z - z * otherRef.y, z * otherRef.x - x * otherRef.z, x * otherRef.y - y * otherRef.x);
}
double vec3::Length() const{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}
double vec3::Angle(const vec3& otherRef) const{
	return acos((vec3(x, y, z).Dot(otherRef)) / (vec3(x, y, z).Length() / otherRef.Length()));
}
vec3 vec3::Norm(double epsilon) const{
	if (Length() < epsilon)return vec3(0, 0, 0);
	else return vec3(x, y, z) / Length();
}
string vec3::ToString() const{
	return string("x: ") + to_string(x) + string(", y: ") + to_string(y) + string(", z: ") + to_string(z);
}
//operators
vec3 operator*(double factor, const vec3& rightRef){
	return vec3(rightRef.x * factor, rightRef.y * factor, rightRef.z*factor);
}
vec3& vec3::operator=(const vec3& otherRef){
	if (&otherRef != this){
		x = otherRef.x;
		y = otherRef.y;
		z = otherRef.z;
	}
	return *this;
}
vec3 vec3::operator+(const vec3& otherRef) const{
	return vec3(x + otherRef.x, y + otherRef.y, z + otherRef.z);
}
vec3 vec3::operator-(const vec3& otherRef) const{
	return vec3(x - otherRef.x, y - otherRef.y, z - otherRef.z);
}
vec3 vec3::operator-() const{
	return vec3(-x, -y, -z);
}
vec3 vec3::operator+() const{
	return *this;
}
vec3 vec3::operator*(double factor) const{
	return vec3(x * factor, y * factor, z*factor);
}
vec3 vec3::operator/(double divisor) const{
	return vec3(x / divisor, y / divisor, z / divisor);
}
vec3& vec3::operator+=(const vec3& otherRef){
	x += otherRef.x;
	y += otherRef.y;
	z += otherRef.z;
	return *this;
}
vec3& vec3::operator-=(const vec3& otherRef){
	x -= otherRef.x;
	y -= otherRef.y;
	z -= otherRef.z;
	return *this;
}
vec3& vec3::operator*=(double factor){
	x *= factor;
	y *= factor;
	z *= factor;
	return *this;
}
vec3& vec3::operator/=(double divisor){
	x /= divisor;
	y /= divisor;
	z /= divisor;
	return *this;
}
bool vec3::operator==(const vec3& otherRef) const{
	return x == otherRef.x && y == otherRef.y && z == otherRef.z;
}
bool vec3::operator!=(const vec3& otherRef) const{
	return !(*this == otherRef);
}

//2D Vector
vec2::vec2(){
}
vec2::vec2(double lx, double ly){
	x = lx;
	y = ly;
}
//methods
double vec2::Dot(const vec2& otherRef) const{
	return x * otherRef.x + y * otherRef.y;
}
vec2 vec2::Orthagonal() const{
	return vec2(-y, x);
}
double vec2::Length() const{
	return sqrt(pow(x, 2) + pow(y, 2));
}
double vec2::Angle(const vec2& otherRef) const{
	return acos((vec2(x, y).Dot(otherRef)) / (vec2(x, y).Length() / otherRef.Length()));
}
vec2 vec2::Norm(double epsilon) const{
	if (Length() < epsilon)return vec2(0, 0);
	else return vec2(x, y) / Length();
}
string vec2::ToString() const{
	return string("x: ") + to_string(x) + string(", y: ") + to_string(y);
}
vec2 vec2::correctUV() const{
	vec2 ret = vec2(x, y);
	if (x > 1){
		int intX = (int)x;
		ret.x = x - intX;
	}
	if (x < 0){
		int intX = (int)x;
		ret.x = -(x - intX);
	}
	if (y > 1){
		int intY = (int)y;
		ret.y = y - intY;
	}
	if (y < 0){
		int intY = (int)y;
		ret.y = -(y - intY);
	}
	return ret;
}
//operators
vec2 operator*(double factor, const vec2& rightRef){
	return vec2(rightRef.x * factor, rightRef.y * factor);
}
vec2& vec2::operator=(const vec2& otherRef){
	if (&otherRef != this){
		x = otherRef.x;
		y = otherRef.y;
	}
	return *this;
}
vec2 vec2::operator+(const vec2& otherRef) const{
	return vec2(x + otherRef.x, y + otherRef.y);
}
vec2 vec2::operator-(const vec2& otherRef) const{
	return vec2(x - otherRef.x, y - otherRef.y);
}
vec2 vec2::operator-() const{
	return vec2(-x, -y);
}
vec2 vec2::operator+() const{
	return *this;
}
vec2 vec2::operator*(double factor) const{
	return vec2(x * factor, y * factor);
}
vec2 vec2::operator/(double divisor) const{
	return vec2(x / divisor, y / divisor);
}
vec2& vec2::operator+=(const vec2& otherRef){
	x += otherRef.x;
	y += otherRef.y;
	return *this;
}
vec2& vec2::operator-=(const vec2& otherRef){
	x -= otherRef.x;
	y -= otherRef.y;
	return *this;
}
vec2& vec2::operator*=(double factor){
	x *= factor;
	y *= factor;
	return *this;
}
vec2& vec2::operator/=(double divisor){
	x /= divisor;
	y /= divisor;
	return *this;
}
bool vec2::operator==(const vec2& otherRef) const{
	return x == otherRef.x && y == otherRef.y;
}
bool vec2::operator!=(const vec2& otherRef) const{
	return !(*this == otherRef);
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
plane::plane(vec3 lN, double lD){
	n = lN;
	d = lD;
}
plane::plane(point3 A, point3 B, point3 C){
	n = (B - A).Cross(C - A).Norm(0.001);
	d = n.Dot(A);
}
intersection plane::lineIts(line ln){
	intersection ret(false, point3(0,0,0), 0);
	double nd = n.Dot(ln.dir);
	if (!(nd == 0)){
		ret.t = (d - n.Dot(ln.orig)) / nd;
		ret.hit = true;
	}
	return ret;
}
intersection plane::rayIts(line ln, bool bfc){
	intersection ret(false, point3(0, 0, 0), 0);
	double nd = n.Dot(ln.dir);
	if (nd<0){
		ret.t = (d - n.Dot(ln.orig)) / nd;
		ret.hit = true;
	}
	else if (nd>0 && bfc == false){
		ret.t = (d - n.Dot(ln.orig)) / nd;
		ret.hit = true;
	}
	return ret;
}


//Intersection
intersection::intersection(){
}
intersection::intersection(bool lHit, point3 lP, double lT){
	hit = lHit;
	p = lP;
	t = lT;
}


//Color
colRGB::colRGB(){
}
colRGB::colRGB(double lx, double ly, double lz){
	red = lx;
	green = ly;
	blue = lz;
}
string colRGB::ToString() const{
	return string("red: ") + to_string(red) + string(", green: ") + to_string(green) + string(", blue: ") + to_string(blue);
}
colRGB operator*(double factor, const colRGB& rightRef){
	return colRGB(rightRef.red * factor, rightRef.green * factor, rightRef.blue*factor);
}
colRGB& colRGB::operator=(const colRGB& otherRef){
	if (&otherRef != this){
		red = otherRef.red;
		green = otherRef.green;
		blue = otherRef.blue;
	}
	return *this;
}
colRGB colRGB::operator+(const colRGB& otherRef) const{
	return colRGB(red + otherRef.red, green + otherRef.green, blue + otherRef.blue);
}
colRGB colRGB::operator-(const colRGB& otherRef) const{
	return colRGB(red - otherRef.red, green - otherRef.green, blue - otherRef.blue);
}
colRGB colRGB::operator-() const{
	return colRGB(-red, -green, -blue);
}
colRGB colRGB::operator+() const{
	return *this;
}
colRGB colRGB::operator*(double factor) const{
	return colRGB(red * factor, green * factor, blue*factor);
}
colRGB colRGB::operator*(const colRGB& otherRef) const{
	return colRGB(((red)*(otherRef.red)),
		((green)*(otherRef.green)),
		((blue)*(otherRef.blue)));
}
colRGB colRGB::operator/(double divisor) const{
	return colRGB(red / divisor, green / divisor, blue / divisor);
}
colRGB& colRGB::operator+=(const colRGB& otherRef){
	red += otherRef.red;
	green += otherRef.green;
	blue += otherRef.blue;
	return *this;
}
colRGB& colRGB::operator-=(const colRGB& otherRef){
	red -= otherRef.red;
	green -= otherRef.green;
	blue -= otherRef.blue;
	return *this;
}
colRGB& colRGB::operator*=(double factor){
	red *= factor;
	green *= factor;
	blue *= factor;
	return *this;
}
colRGB& colRGB::operator*=(const colRGB& otherRef){
	red = ((red)*(otherRef.red));
	green = ((green)*(otherRef.green));
	blue = ((blue)*(otherRef.blue));
	return *this;
}
colRGB& colRGB::operator/=(double divisor){
	red /= divisor;
	green /= divisor;
	blue /= divisor;
	return *this;
}
bool colRGB::operator==(const colRGB& otherRef) const{
	return red == otherRef.red && green == otherRef.green && blue == otherRef.blue;
}
bool colRGB::operator!=(const colRGB& otherRef) const{
	return !(*this == otherRef);
}