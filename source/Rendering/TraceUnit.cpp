#include "TraceUnit.hpp"													

TraceUnit::TraceUnit()
{
	// nothing to create
}

TraceUnit::~TraceUnit()																						
{
	delete m_BvhPtr;
	m_BvhPtr = nullptr;
}

void TraceUnit::setScene(Scene *sc){
	m_ScenePtr = sc;
}

void TraceUnit::init(int camWidth, int camHeight){
	//setup blank image
	clearImage(camWidth, camHeight);
	//setup Ray Map
	updateRayMap();
	//setup tiles
	int tileSizeX = 128, tileSizeY = 128;
	tiles =	setupTiles(camWidth, camHeight, tileSizeX, tileSizeY);

	//setup BVH
	cout << "building QBVH tree...." << endl;
	m_BvhPtr = new BVH();
	m_BvhPtr->Build(m_ScenePtr);
	cout << "QBVH tree built!" << endl;
}

void TraceUnit::clearImage(int camWidth, int camHeight)
{
	m_Image = Texture("Render result", camWidth, camHeight);
	m_Image.setInterpolationMode(INTPOL_PIXELS);
	m_Image.setQuadraticFittingMode(FIT_STRETCHXY);
}

void TraceUnit::updateRayMap()
{
	rayMap = m_ScenePtr->cam.getRayMap(p_MaxBounces);
}

Texture *TraceUnit::getImage()
{
	return &m_Image;
}

//returns true if a sample is completed, otherwise false to allow avoiding freezing
bool TraceUnit::renderNextChunk() 
{
	vector<thread> tileThreads;

	int availableThreads = thread::hardware_concurrency();

	for (int i = 0; i < availableThreads; i++)
	{
		if (m_currentTile < (signed int)tiles.size())
			tileThreads.push_back(std::thread(&TraceUnit::renderTile, this, m_currentTile));
		m_currentTile++;
	}
	int activeThreads = tileThreads.size();
	while (activeThreads>0)
	{
		for (size_t i = 0; i < tileThreads.size(); i++)
		{
			if (tileThreads[i].joinable())
			{
				tileThreads[i].join();
				activeThreads--;
			}
		}
	}

	if (m_currentTile >= (signed int)tiles.size())
	{
		m_currentTile = 0;
		m_samplesRendered++;
		return true;
	}
	else return false;
}

void TraceUnit::renderTile(int tileIndex)
{
	tile thisTile = tiles[tileIndex];
	for (int x = thisTile.posX; x < thisTile.posX + thisTile.scaleX; x++)
	{
		for (int y = thisTile.posY; y < thisTile.posY + thisTile.scaleY; y++)
		{
			colRGB current = m_Image.getRGB(x, y);
			float t;
			colRGB next = ((current*(float)m_samplesRendered) + raycast(rayMap[x][y], t)) / (float)(m_samplesRendered + 1);
			m_Image.setRGB(next, x, y);
		}
	}
}

colRGB TraceUnit::raycast(Ray ray, float &t)
{
	DifferentialGeometry closest;
	closest.i.hit = false;
	bool hasHit=false;
	float minT = ray.isPrimary ? 1.0f : 0.0001f;
	for (size_t i = 0; i < m_BvhPtr->unsortedIndices.size(); i++)
	{
		shapeNodeIdx idxStruct = m_BvhPtr->unsortedIndices[i];
		m_ScenePtr->shapes[idxStruct.shapeIdx]->getIntersection(idxStruct.subIdx1, idxStruct.subIdx2, ray, closest, minT, m_BackfaceCulling);
		
		if (closest.i.hit)
		{
			hasHit = true;
		}
	}
	traverseBVH(ray, m_BvhPtr->outerNode, closest, hasHit, minT);

	closest.dir = ray.ln.dir;
	closest.bounces = ray.bounces;

	//shade closest intersection
	if (hasHit == false)
	{
		closest.mat = materialPointer::BACKGROUND_MATERIAL;
	}
	else
	{
		t = closest.i.t / closest.dir.Length();
	}
	return shade(closest);
}
void TraceUnit::traverseBVH(Ray &ray, bvhNode *node, DifferentialGeometry &dg, bool &hasHit, float minT)
{
	//intersect with volume and check if its closer than the already found distance
	bool aabbHit = false;
	if (hasHit)
	{
		aabbHit = node->bounds.intersect(ray, dg.i.t);
	}
	else
	{
		aabbHit = node->bounds.intersect(ray);
	}
	if (aabbHit)
	{
		//solve this nodes assigned shapes intersections
		for (size_t i = 0; i < node->Indices.size(); i++)
		{
			shapeNodeIdx idxStruct = node->Indices[i];
			m_ScenePtr->shapes[idxStruct.shapeIdx]->getIntersection(idxStruct.subIdx1, idxStruct.subIdx2, ray, dg, minT, m_BackfaceCulling);
			if (dg.i.hit)
			{
				hasHit = true;
			}
		}
		//traverse hierachy for existing child nodes
		if (!(node->Child0 == nullptr))
		{
			traverseBVH(ray, node->Child0, dg, hasHit, minT);
		}
		if (!(node->Child1 == nullptr))
		{
			traverseBVH(ray, node->Child1, dg, hasHit, minT);
		}
		if (!(node->Child2 == nullptr))
		{
			traverseBVH(ray, node->Child2, dg, hasHit, minT);
		}
		if (!(node->Child3 == nullptr))
		{
			traverseBVH(ray, node->Child3, dg, hasHit, minT);
		}
	}
}
bool TraceUnit::shadowRay(line ln)
{
	bool hasHit = false;
	for (size_t i = 0; i < m_BvhPtr->unsortedIndices.size(); i++)
	{
		shapeNodeIdx idxStruct = m_BvhPtr->unsortedIndices[i];
		if (m_ScenePtr->shapes[idxStruct.shapeIdx]->castsShadow())
		{
			if (m_ScenePtr->shapes[idxStruct.shapeIdx]->shadowIntersection(idxStruct.subIdx1, idxStruct.subIdx2, ln))
				hasHit = true;
		}
	}
	if (!hasHit)
	{
		Ray ray;
		ray.ln = ln;
		ray.precalculate();
		shadowTraverseBVH(ray, m_BvhPtr->outerNode, hasHit);
	}
	return hasHit;
}
void TraceUnit::shadowTraverseBVH(Ray &ray, bvhNode *node, bool &hasHit)
{
	if (node->bounds.intersect(ray))
	{
		for (size_t i = 0; i < node->Indices.size(); i++)
		{
			shapeNodeIdx idxStruct = node->Indices[i];
			if (m_ScenePtr->shapes[idxStruct.shapeIdx]->castsShadow())
			{
				if (m_ScenePtr->shapes[idxStruct.shapeIdx]->shadowIntersection(idxStruct.subIdx1, idxStruct.subIdx2, ray.ln))
					hasHit = true;
			}
		}
		//traverse hierachy for existing child nodes
		if (!(node->Child0 == nullptr) && !hasHit)
		{
			shadowTraverseBVH(ray, node->Child0, hasHit);
		}										 
		if (!(node->Child1 == nullptr) && !hasHit)
		{										 
			shadowTraverseBVH(ray, node->Child1, hasHit);
		}
		if (!(node->Child2 == nullptr) && !hasHit)
		{
			shadowTraverseBVH(ray, node->Child2, hasHit);
		}
		if (!(node->Child3 == nullptr) && !hasHit)
		{
			shadowTraverseBVH(ray, node->Child3, hasHit);
		}
	}
}

colRGB TraceUnit::shade(DifferentialGeometry dg)
{
	shader *mat = m_ScenePtr->materials[dg.mat];
	return mat->shade(dg, m_ScenePtr, this);
}

float TraceUnit::getLightIntensity(light *L, point3 P)
{
	float ret = 0.f;

	if (L->getType() == LightType::LIGHT_POINT)
	{
		if (!shadowRay(line(P, (L->getPosition() - P).Norm(0.00001f))))
		{
			ret = 1.f;
		}
	}
		
	if (L->getType() == LightType::LIGHT_AREA)
	{
		float sampleDifferential = (1.0f / (float)((AreaLight*)L)->samplesSq);
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0, sampleDifferential);
		for (float x = 0.f; x < 1.f; x += sampleDifferential)
		{
			for (float y = 0.f; y < 1.f; y += sampleDifferential)
			{
				point3 pos = ((AreaLight*)L)->pos + (x + (float)dis(gen))*((AreaLight*)L)->dir1 + (y + (float)dis(gen))*((AreaLight*)L)->dir2;
				float hitValue = 1.f / (float)(((AreaLight*)L)->samplesSq*((AreaLight*)L)->samplesSq);
				if (!shadowRay(line(P, (pos - P).Norm(0.00001f))))
				{
					ret += hitValue;
				}
			}
		}
	}
	ret *= L->getIntensity();
	return ret;
}


vector<tile> TraceUnit::setupTiles(int camWidth, int camHeight, int tileSizeX, int tileSizeY)
{
	vector<tile> tiles;
	for (int x = 0; x < camWidth; x += tileSizeX)
	{
		for (int y = 0; y < camHeight; y += tileSizeY)
		{
			tile T;
			T.posX = x;
			T.posY = y;
			if (x + tileSizeX > camWidth)T.scaleX = camWidth - x;
			else T.scaleX = tileSizeX;
			if (y + tileSizeY > camHeight)T.scaleY = camHeight - y;
			else T.scaleY = tileSizeY;
			tiles.push_back(T);
		}
	}
	return tiles;
}