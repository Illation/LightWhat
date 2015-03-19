#pragma once
#include "Scene.hpp"
#include "commonR.hpp"
#include "shader.hpp"
#include "shaders/Background.hpp"
#include "shaders/DiffuseBRDF.hpp"
#include "shaders/EmissionBRDF.hpp"
#include "shaders/GlassBRDF.hpp"
#include "shaders/GlossyBRDF.hpp"
#include "shaders/Mix.hpp"
#include "BVH.hpp"
#include <thread>
#include <ctime>
#include <random>

class Renderer
{
public:				
	Renderer();
	virtual ~Renderer();

	bool renderNextChunk();
	Texture *getImage();
	void setScene(Scene *sc);
	void init(int camWidth, int camHeight);
	void clearImage(int camWidth, int camHeight);
	void updateRayMap();

	float getLightIntensity(light *L, point3 P);
	colRGB raycast(Ray, float &t);

	const int p_MaxBounces = 8;
	bool m_BackfaceCulling = false;

	int m_samplesRendered = 0;
private:
	vector<tile> setupTiles(int camWidth, int camHeight, int tileSizeX, int tileSizeY);

	void renderTile(int tileIndex);

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


	Renderer(const Renderer& tRef);
	Renderer& operator=(const Renderer& tRef);

};
