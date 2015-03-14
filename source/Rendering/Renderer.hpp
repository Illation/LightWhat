#pragma once
#include "Scene.hpp"
#include "commonR.hpp"
#include "BVH.hpp"
#include <thread>
#include <ctime>
class Renderer
{
public:				
	Renderer();
	virtual ~Renderer();

	bool renderNextChunk();
	Texture *getImage();
	void setScene(Scene *sc);
	void init(int camWidth, int camHeight);

	const int p_MaxBounces = 8;
	bool m_BackfaceCulling = true;
private:
	vector<tile> setupTiles(int camWidth, int camHeight, int tileSizeX, int tileSizeY);

	void renderTile(int tileIndex);

	colRGB raycast(Ray);
	void traverseBVH(Ray &ray, bvhNode *node, DifferentialGeometry &dg, bool &hasHit, float minT);
	bool shadowRay(line);
	void shadowTraverseBVH(Ray &ray, bvhNode *node, bool &hasHit);

	colRGB shade(DifferentialGeometry);

	Texture m_Image;
	vector<vector<Ray> > rayMap;
	vector<tile> tiles;
	Scene *m_ScenePtr = nullptr;
	BVH *m_BvhPtr = nullptr;

	int m_currentTile = 0;
	int m_samplesRendered = 0;


	Renderer(const Renderer& tRef);
	Renderer& operator=(const Renderer& tRef);

};
