#include "Mesh.h"

tri::tri(){
}
tri::tri(int va, int vb, int vc, int na, int nb, int nc){
	vertA = va;
	vertB = vb;
	vertC = vc;
	normA = na;
	normB = nb;
	normC = nc;
}
void tri::setUV(int uva, int uvb, int uvc){
	uvA = uva;
	uvB = uvb;
	uvC = uvc;
}
void tri::precalculate(vec3 a, vec3 b, vec3 c){
	triPlane = plane(a, b, c);
	v0 = c - a;
	v1 = b - a;
	dot00 = v0.Dot(v0);
	dot01 = v0.Dot(v1);
	dot11 = v1.Dot(v1);
	invDenom = (dot00 * dot11 - dot01 * dot01);
}

polylist::polylist(){
}
polylist::polylist(size_t materialIndex, bool lUV){
	matIndex = materialIndex;
	hasUV = lUV;
}

Mesh::Mesh(){
}
Mesh::~Mesh(){
}

void Mesh::getIntersection(Ray ray, DifferentialGeometry &closest, double minT, bool bfc){
	intersection its;
	for (size_t i = 0; i < m_TriLists.size(); i++)
	{
		polylist plist = m_TriLists[i];
		for (size_t j = 0; j < plist.triangles.size(); j++)
		{
			tri thisTri = plist.triangles[j];
			point3 fVertA = m_VertexList[thisTri.vertA];
			its = thisTri.triPlane.rayIts(ray.ln, bfc);
			if (its.hit == true)
			{
				if ((its.t < closest.i.t || closest.i.hit == false) && its.t>minT) // check if its is closest intersection
				{
					//calculate baryacentric coordinates
					its.p = ray.ln.orig + ray.ln.dir*its.t;
					vec3 v2 = its.p - fVertA;
					double dot02 = thisTri.v0.Dot(v2);
					double dot12 = thisTri.v1.Dot(v2);

					double u = (thisTri.dot11 * dot02 - thisTri.dot01 * dot12) / thisTri.invDenom;
					if (u >= 0)
					{
						double v = (thisTri.dot00 * dot12 - thisTri.dot01 * dot02) / thisTri.invDenom;
						if (v < 0) its.hit = false;
						if (u + v > 1) its.hit = false;
						if (its.hit)
						{
							closest.i = its;
							closest.mat = plist.matIndex;

							//smooth normals
							vec3 n1 = m_NormalList[thisTri.normA];
							vec3 n2 = m_NormalList[thisTri.normB];
							vec3 n3 = m_NormalList[thisTri.normC];
							vec3 N = n1 + (n3 - n1)*u + (n2 - n1)*v;
							closest.n = N.Norm(0.00001);

							//uv map
							closest.u = u;
							closest.v = v;
						}
					}
					else its.hit = false;
				}
				else its.hit = false;
			}
		}
	}
}
bool Mesh::shadowIntersection(line ln){
	intersection its;
	bool hit = false;
	double shadowLength = ln.dir.Length();
	for (size_t i = 0; i < m_TriLists.size(); i++)
	{
		polylist plist = m_TriLists[i];
		for (size_t j = 0; j < plist.triangles.size(); j++)
		{
			tri thisTri = plist.triangles[j];
			point3 fVertA = m_VertexList[thisTri.vertA];
			its = thisTri.triPlane.lineIts(ln);
			if (its.hit == true)
			{
				if (its.t>0.001 && its.t < shadowLength) // avoid z fighting and objects behind light
				{
					//calculate baryacentric coordinates
					its.p = ln.orig + ln.dir*its.t;
					vec3 v2 = its.p - fVertA;

					double dot02 = thisTri.v0.Dot(v2);
					double dot12 = thisTri.v1.Dot(v2);

					double u = (thisTri.dot11 * dot02 - thisTri.dot01 * dot12) / thisTri.invDenom;
					if (u >= 0)
					{
						double v = (thisTri.dot00 * dot12 - thisTri.dot01 * dot02) / thisTri.invDenom;
						if (v < 0) its.hit = false;
						if (u + v > 1) its.hit = false;
						if (its.hit)
						{
							hit = true;
						}
					}
				}
			}
		}
	}
	return hit;
}
point3 Mesh::getPosition()
{
	return m_Origin;
}
shapeType Mesh::getType(){
	return MESH;
}

void Mesh::addVertex(point3 vert){
	m_VertexList.push_back(vert);
}
void Mesh::addNormal(vec3 norm){
	m_NormalList.push_back(norm);
}
void Mesh::addPolyList(size_t mIndex, bool hasUV){
	m_TriLists.push_back(polylist(mIndex, hasUV));
}
void Mesh::createTri(int a, int b, int c, int x, int y, int z, size_t index){
	
	if (index < m_TriLists.size())
	{
		vec3 vertA, vertB, vertC;
		for (size_t i = 0; i < m_VertexList.size(); i++){
			if (a == i)vertA = m_VertexList[i];
			if (b == i)vertB = m_VertexList[i];
			if (c == i)vertC = m_VertexList[i];
		}

		tri thisTri = tri(a, b, c, x, y, z);
		thisTri.precalculate(vertA, vertB, vertC);
		m_TriLists[index].triangles.push_back(thisTri);
	}
	else
	{
		cout << "polylist does not exist!" << endl;
	}
}
void Mesh::createTri(tri f, size_t index){
	if (index < m_TriLists.size())
	{
		vec3 vertA, vertB, vertC;
		for (size_t i = 0; i < m_VertexList.size(); i++){
			if (f.vertA == i)vertA = m_VertexList[i];
			if (f.vertB == i)vertB = m_VertexList[i];
			if (f.vertC == i)vertC = m_VertexList[i];
		}
		f.precalculate(vertA, vertB, vertC);
		m_TriLists[index].triangles.push_back(f);
	}
	else
	{
		cout << "polylist does not exist!" << endl;
	}
}

void Mesh::setPosition(double x, double y, double z){
	m_Origin = point3(x, y, z);
}
void Mesh::setPosition(point3 pos){
	m_Origin = pos;
}
void Mesh::setMaterial(size_t materialIndex, size_t index){
	if (index < m_TriLists.size()){
		m_TriLists[index].matIndex = materialIndex;
	}
	else cout << "polylist does not exist!" << endl;
}

vector <point3>Mesh::getVertices(){
	return m_VertexList;
}
vector <vec3>Mesh::getNormals(){
	return m_NormalList;
}
vector <polylist>Mesh::getPolyLists()
{
	return m_TriLists;
}
int Mesh::getVertCount(){
	return m_VertexList.size();
}
int Mesh::getTriCount(){
	int ret = 0;
	for (size_t i = 0; i < m_TriLists.size(); i++){
		ret += m_TriLists[i].triangles.size();
	}
	return ret;
}