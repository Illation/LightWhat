#pragma once

#include <string>
using namespace std;

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

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
	vec3(double x, double y, double z);

	double x;
	double y;
	double z;

	friend vec3	operator*(double factor, const vec3& right);

	//methods
	double Dot(const vec3& other) const;								
	double Length() const;											
	double Angle(const vec3& other) const;						
	vec3 Norm(double epsilon = 0.001) const;				
	vec3 Cross(const vec3& other) const;
	string ToString() const;

	//Operators	
	vec3&			operator= (const vec3& other);
	vec3			operator+ (const vec3& other) const;
	vec3			operator- (const vec3& other) const;
	vec3			operator- () const;
	vec3			operator+ () const;
	vec3			operator* (double factor) const;
	vec3			operator/ (double divisor) const;
	vec3&			operator+=(const vec3& other);
	vec3&			operator-=(const vec3& other);
	vec3&			operator*=(double factor);
	vec3&			operator/=(double divisor);
	bool			operator==(const vec3& other) const;
	bool			operator!=(const vec3& other) const;
};

struct vec2
{
public:
	vec2();
	vec2(double x, double y);

	double x;
	double y;

	friend vec2	operator*(double factor, const vec2& right);

	//methods
	double Dot(const vec2& other) const;
	double Length() const;
	double Angle(const vec2& other) const;
	vec2 Norm(double epsilon = 0.001) const;
	vec2 Orthagonal() const;
	string ToString() const;

	//Operators	
	vec2&			operator= (const vec2& other);
	vec2			operator+ (const vec2& other) const;
	vec2			operator- (const vec2& other) const;
	vec2			operator- () const;
	vec2			operator+ () const;
	vec2			operator* (double factor) const;
	vec2			operator/ (double divisor) const;
	vec2&			operator+=(const vec2& other);
	vec2&			operator-=(const vec2& other);
	vec2&			operator*=(double factor);
	vec2&			operator/=(double divisor);
	bool			operator==(const vec2& other) const;
	bool			operator!=(const vec2& other) const;
};

struct line{
	line();
	line(point3, vec3);

	point3 orig;
	vec3 dir;
};

struct plane{
	plane();
	plane(vec3, double);
	plane(point3, point3, point3);

	vec3 n;
	double d;

	intersection lineIts(line);
	intersection rayIts(line, bool);
};

struct intersection{
	intersection();
	intersection(bool, point3, double);
	bool hit;
	point3 p;
	double t;
};

struct colRGB
{
public:
	colRGB();
	colRGB(double red, double green, double blue);

	double red;
	double green;
	double blue;

	string			ToString() const;

	colRGB&		operator= (const colRGB& other);
	colRGB		operator+ (const colRGB& other) const;
	colRGB		operator- (const colRGB& other) const;
	colRGB		operator- () const;
	colRGB		operator+ () const;
	colRGB		operator* (double factor) const;
	colRGB		operator* (const colRGB& other) const;
	colRGB		operator/ (double divisor) const;
	colRGB&		operator+=(const colRGB& other);
	colRGB&		operator-=(const colRGB& other);
	colRGB&		operator*=(double factor);
	colRGB&		operator*=(const colRGB& other);
	colRGB&		operator/=(double divisor);
	bool		operator==(const colRGB& other) const;
	bool		operator!=(const colRGB& other) const;
};