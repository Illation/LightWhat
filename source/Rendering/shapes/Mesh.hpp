#pragma once
#include "../commonR.hpp"
#include "../shape.hpp"
#include <algorithm>
#include <vector>
using namespace std;

struct tri
{
	tri();
	tri(int va, int vb, int vc, int na, int nb, int nc);
	void setUV(int uva, int uvb, int uvc);
	void precalculate(vec3 a, vec3 b, vec3 c);

	int vertA, vertB, vertC;
	int normA, normB, normC;
	int uvA, uvB, uvC;

	plane triPlane;
	vec3 v0, v1;
	double dot00, dot01, dot11, invDenom;
};

struct polylist{
	polylist();
	polylist(size_t, bool hasUV);

	bool hasUV;
	size_t matIndex;
	vector <tri> triangles;
};

struct UVset{
	UVset();
	UVset(string, bool);

	vector<point2> coords;
	bool isActive;
	string name;
};

class Mesh : public shape
{
public:
	Mesh();
	virtual ~Mesh( );


	// C++11 make the class non-copyable
	Mesh( const Mesh& ) = delete;
	Mesh& operator=( const Mesh& ) = delete;

	//shape methods
	void getIntersection(Ray ray, DifferentialGeometry &closest, double minT, bool bfc);
	point3 getPosition();
	AABB getBoundingBox(size_t subShapeIdx, size_t subShapeIdx2);
	point3 getObjectCenter(size_t subShapeIdx, size_t subShapeIdx2);
	bool shadowIntersection(line ln);
	shapeType getType();

	//methods
	void addVertex(point3 vert);
	void addNormal(vec3 norm);
	void addTangent(vec3 tan);
	void addBiTangent(vec3 bitan);
	void addPolyList(size_t mIndex, bool hasUV);
	void addUVset(string Name, bool isActive);
	void addUV(point2 coords, size_t index);
	void createTri(int a, int b, int c, int x, int y, int z, size_t index);
	void createTri(tri f, size_t index);

	void setPosition(double x, double y, double z);
	void setPosition(point3 pos);
	void setMaterial(size_t materialIndex, size_t index);
	void setUVactive(size_t uvIndex, bool isActive);

	vector <point3>getVertices();
	vector <vec3>getNormals();
	vector <polylist>getPolyLists();
	vector <UVset>getUVCoordinates();
	string getName();

	int getVertCount();
	int getTriCount();
	int getPListCount();

	bool hasTangentSpace = false;
	vector <polylist>m_TriLists;
private: 
	vector <point3>m_VertexList;
	vector <vec3>m_NormalList;
	vector <vec3>m_TangentList;
	vector <vec3>m_BiTangentList;
	vector <UVset>m_UVs;
	point3 m_Origin;
	string m_Name;
};