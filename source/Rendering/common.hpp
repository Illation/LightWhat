#pragma once

#include <string>
#include <algorithm>
//using namespace std;

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062f

struct vec3;
typedef vec3 point3;
struct vec2;
typedef vec2 point2;
struct line;
struct plane;
struct intersection;
struct colRGB;
//Vectors
struct vec3
{
public:
	vec3();
	vec3(float x, float y, float z);

	float x;
	float y;
	float z;

	inline friend vec3	operator*(float factor, const vec3& rightRef){
		return vec3(rightRef.x * factor, rightRef.y * factor, rightRef.z*factor);
	}

	//methods
	inline float Dot(const vec3& otherRef) const{
		return x * otherRef.x + y * otherRef.y + z*otherRef.z;
	}
	inline float Length() const{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}
	inline float Angle(const vec3& otherRef) const{
		return acos((vec3(x, y, z).Dot(otherRef)) / (vec3(x, y, z).Length() / otherRef.Length()));
	}
	inline vec3 Norm(float epsilon = 0.001) const{
		if (Length() < epsilon)return vec3(0, 0, 0);
		else return vec3(x, y, z) / Length();
	}
	inline vec3 Cross(const vec3& otherRef) const{
		return vec3(y * otherRef.z - z * otherRef.y, z * otherRef.x - x * otherRef.z, x * otherRef.y - y * otherRef.x);
	}
	inline std::string ToString() const{
		using namespace std;
		return string("x: ") + to_string(x) + string(", y: ") + to_string(y) + string(", z: ") + to_string(z);
	}

	//Operators	
	inline vec3&			operator= (const vec3& otherRef){
		if (&otherRef != this){
			x = otherRef.x;
			y = otherRef.y;
			z = otherRef.z;
		}
		return *this;
	}
	inline vec3			operator+ (const vec3& otherRef) const{
		return vec3(x + otherRef.x, y + otherRef.y, z + otherRef.z);
	}
	inline vec3			operator- (const vec3& otherRef) const{
		return vec3(x - otherRef.x, y - otherRef.y, z - otherRef.z);
	}
	inline vec3			operator- () const{
		return vec3(-x, -y, -z);
	}
	inline vec3			operator+ () const{
		return *this;
	}
	inline vec3			operator* (float factor) const{
		return vec3(x * factor, y * factor, z*factor);
	}
	inline vec3			operator/ (float divisor) const{
		return vec3(x / divisor, y / divisor, z / divisor);
	}
	inline vec3&			operator+=(const vec3& otherRef){
		x += otherRef.x;
		y += otherRef.y;
		z += otherRef.z;
		return *this;
	}
	inline vec3&			operator-=(const vec3& otherRef){
		x -= otherRef.x;
		y -= otherRef.y;
		z -= otherRef.z;
		return *this;
	}
	inline vec3&			operator*=(float factor){
		x *= factor;
		y *= factor;
		z *= factor;
		return *this;
	}
	inline vec3&			operator/=(float divisor){
		x /= divisor;
		y /= divisor;
		z /= divisor;
		return *this;
	}
	inline bool			operator==(const vec3& otherRef) const{
		return x == otherRef.x && y == otherRef.y && z == otherRef.z;
	}
	inline bool			operator!=(const vec3& otherRef) const{
		return !(*this == otherRef);
	}
};

struct vec2
{
public:
	vec2();
	vec2(float x, float y);

	float x;
	float y;

	inline friend vec2	operator*(float factor, const vec2& rightRef){
		return vec2(rightRef.x * factor, rightRef.y * factor);
	}

	//methods
	inline float Dot(const vec2& otherRef) const{
		return x * otherRef.x + y * otherRef.y;
	}
	inline float Length() const{
		return sqrt(pow(x, 2) + pow(y, 2));
	}
	inline float Angle(const vec2& otherRef) const{
		return acos((vec2(x, y).Dot(otherRef)) / (vec2(x, y).Length() / otherRef.Length()));
	}
	inline vec2 Norm(float epsilon = 0.001) const{
		if (Length() < epsilon)return vec2(0, 0);
		else return vec2(x, y) / Length();
	}
	inline vec2 Orthagonal() const{
		return vec2(-y, x);
	}
	inline std::string ToString() const{
		using namespace std;
		return string("x: ") + to_string(x) + string(", y: ") + to_string(y);
	}
	inline vec2 correctUV() const{
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

	//Operators	
	inline vec2&			operator= (const vec2& otherRef){
		if (&otherRef != this){
			x = otherRef.x;
			y = otherRef.y;
		}
		return *this;
	}
	inline vec2			operator+ (const vec2& otherRef) const{
		return vec2(x + otherRef.x, y + otherRef.y);
	}
	inline vec2			operator- (const vec2& otherRef) const{
		return vec2(x - otherRef.x, y - otherRef.y);
	}
	inline vec2			operator- () const{
		return vec2(-x, -y);
	}
	inline vec2			operator+ () const{
		return *this;
	}
	inline vec2			operator* (float factor) const{
		return vec2(x * factor, y * factor);
	}
	inline vec2			operator/ (float divisor) const{
		return vec2(x / divisor, y / divisor);
	}
	inline vec2&			operator+=(const vec2& otherRef){
		x += otherRef.x;
		y += otherRef.y;
		return *this;
	}
	inline vec2&			operator-=(const vec2& otherRef){
		x -= otherRef.x;
		y -= otherRef.y;
		return *this;
	}
	inline vec2&			operator*=(float factor){
		x *= factor;
		y *= factor;
		return *this;
	}
	inline vec2&			operator/=(float divisor){
		x /= divisor;
		y /= divisor;
		return *this;
	}
	inline bool			operator==(const vec2& otherRef) const{
		return x == otherRef.x && y == otherRef.y;
	}
	inline bool			operator!=(const vec2& otherRef) const{
		return !(*this == otherRef);
	}
};

struct line{
	line();
	line(point3, vec3);

	point3 orig;
	vec3 dir;
};

struct intersection{
	intersection();
	intersection(bool, point3, float);
	bool hit;
	point3 p;
	float t;
};

struct plane{
	plane();
	plane(vec3, float);
	plane(point3, point3, point3);

	vec3 n;
	float d;

	inline intersection lineIts(line ln){
		intersection ret(false, point3(0, 0, 0), 0);
		float nd = n.Dot(ln.dir);
		if (!(nd == 0)){
			ret.t = (d - n.Dot(ln.orig)) / nd;
			ret.hit = true;
		}
		return ret;
	}
	inline intersection rayIts(line ln, bool bfc){
		intersection ret(false, point3(0, 0, 0), 0);
		float nd = n.Dot(ln.dir);
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
};

struct colRGB
{
public:
	colRGB();
	colRGB(float red, float green, float blue);

	float red;
	float green;
	float blue;

	inline std::string			ToString() const{
		using namespace std;
		return string("red: ") + to_string(red) + string(", green: ") + to_string(green) + string(", blue: ") + to_string(blue);
	}

	inline friend colRGB operator*(float factor, const colRGB& rightRef){
		return colRGB(rightRef.red * factor, rightRef.green * factor, rightRef.blue*factor);
	}

	inline colRGB&		operator= (const colRGB& otherRef){
		if (&otherRef != this){
			red = otherRef.red;
			green = otherRef.green;
			blue = otherRef.blue;
		}
		return *this;
	}
	inline colRGB		operator+ (const colRGB& otherRef) const{
		return colRGB(red + otherRef.red, green + otherRef.green, blue + otherRef.blue);
	}
	inline colRGB		operator- (const colRGB& otherRef) const{
		return colRGB(red - otherRef.red, green - otherRef.green, blue - otherRef.blue);
	}
	inline colRGB		operator- () const{
		return colRGB(-red, -green, -blue);
	}
	inline colRGB		operator+ () const{
		return *this;
	}
	inline colRGB		operator* (float factor) const{
		return colRGB(red * factor, green * factor, blue*factor);
	}
	inline colRGB		operator* (const colRGB& otherRef) const{
		return colRGB(((red)*(otherRef.red)),
			((green)*(otherRef.green)),
			((blue)*(otherRef.blue)));
	}
	inline colRGB		operator/ (float divisor) const{
		return colRGB(red / divisor, green / divisor, blue / divisor);
	}
	inline colRGB&		operator+=(const colRGB& otherRef){
		red += otherRef.red;
		green += otherRef.green;
		blue += otherRef.blue;
		return *this;
	}
	inline colRGB&		operator-=(const colRGB& otherRef){
		red -= otherRef.red;
		green -= otherRef.green;
		blue -= otherRef.blue;
		return *this;
	}
	inline colRGB&		operator*=(float factor){
		red *= factor;
		green *= factor;
		blue *= factor;
		return *this;
	}
	inline colRGB&		operator*=(const colRGB& otherRef){
		red = ((red)*(otherRef.red));
		green = ((green)*(otherRef.green));
		blue = ((blue)*(otherRef.blue));
		return *this;
	}
	inline colRGB&		operator/=(float divisor){
		red /= divisor;
		green /= divisor;
		blue /= divisor;
		return *this;
	}
	inline bool		operator==(const colRGB& otherRef) const{
		return red == otherRef.red && green == otherRef.green && blue == otherRef.blue;
	}
	inline bool		operator!=(const colRGB& otherRef) const{
		return !(*this == otherRef);
	}
};