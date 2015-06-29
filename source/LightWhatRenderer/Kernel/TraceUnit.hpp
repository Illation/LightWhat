#pragma once
#include "LWScene.hpp"
#include "../Utility/commonR.hpp"
#include "../Shading/shader.hpp"
#include "../Shading/shaders/Background.hpp"
#include "../Shading/shaders/DiffuseBRDF.hpp"
#include "../Shading/shaders/EmissionBRDF.hpp"
#include "../Shading/shaders/GlassBRDF.hpp"
#include "../Shading/shaders/GlossyBRDF.hpp"
#include "../Shading/shaders/Mix.hpp"
#include "../Geometry/Acceleration/BVH.hpp"
#include <thread>
#include <ctime>
#include <random>

class TraceUnit
{
public:				
	TraceUnit();
	virtual ~TraceUnit();

	bool renderNextChunk();
	void setScene(LWScene *sc);
	void Init(Texture *imagePtr, int camWidth, int camHeight);
	void clearImage(int camWidth, int camHeight);
	void updateRayMap();

	float getLightIntensity(light *L, point3 P);
	colRGB raycast(Ray, float &t);

	int p_MaxBounces = 8;
	bool m_BackfaceCulling = false;

	int m_samplesRendered = 0;
private:
	vector<tile> setupTiles(int camWidth, int camHeight, int tileSizeX, int tileSizeY);

	void renderTile(int tileIndex);

	void traverseBVH(Ray &ray, bvhNode *node, DifferentialGeometry &dg, bool &hasHit, float minT);
	bool shadowRay(line);
	void shadowTraverseBVH(Ray &ray, bvhNode *node, bool &hasHit);

	colRGB shade(DifferentialGeometry);

	Texture *m_ImagePtr = nullptr;
	vector<vector<Ray> > rayMap;
	vector<tile> tiles;
	LWScene *m_ScenePtr = nullptr;
	BVH *m_BvhPtr = nullptr;

	int m_currentTile = 0;


	TraceUnit(const TraceUnit& tRef);
	TraceUnit& operator=(const TraceUnit& tRef);

};
