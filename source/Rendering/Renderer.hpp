#pragma once
#include "Scene.hpp"
#include "commonR.hpp"
#include "BVH.hpp"
#include <ctime>
class Renderer
{
public:				
	Renderer();
	virtual ~Renderer();

	bool renderNextRow();
	Texture *getImage();
	void setScene(Scene *sc);
	void init(int camWidth, int camHeight);

	colRGB raycast(Ray);
	void traverseBVH(Ray &ray, bvhNode *node, DifferentialGeometry &dg, bool &hasHit, double minT);
	bool shadowRay(line);
	void shadowTraverseBVH(Ray &ray, bvhNode *node, bool &hasHit);
	colRGB shade(DifferentialGeometry);

	const int p_MaxBounces = 8;
	bool m_BackfaceCulling = false;
private:

	Texture m_Image;
	vector<vector<Ray> > rayMap;
	Scene *m_ScenePtr = nullptr;
	BVH *m_BvhPtr = nullptr;

	int m_currentRow = 0;
	int m_samplesRendered = 0;


	Renderer(const Renderer& tRef);
	Renderer& operator=(const Renderer& tRef);

};
