#include "Renderer.hpp"													

Renderer::Renderer()
{
	// nothing to create
}

Renderer::~Renderer()																						
{
	delete m_BvhPtr;
	m_BvhPtr = nullptr;
}

void Renderer::setScene(Scene *sc){
	m_ScenePtr = sc;
}

void Renderer::init(int camWidth, int camHeight){
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

void Renderer::clearImage(int camWidth, int camHeight)
{
	m_Image = Texture("Render result", camWidth, camHeight);
	m_Image.setInterpolationMode(INTPOL_PIXELS);
	m_Image.setQuadraticFittingMode(FIT_STRETCHXY);
}

void Renderer::updateRayMap()
{
	rayMap = m_ScenePtr->cam.getRayMap(p_MaxBounces);
}

Texture *Renderer::getImage()
{
	return &m_Image;
}

bool Renderer::renderNextChunk()
{
	vector<thread> tileThreads;

	int availableThreads = thread::hardware_concurrency();

	for (int i = 0; i < availableThreads; i++)
	{
		if (m_currentTile < (signed int)tiles.size())
			tileThreads.push_back(std::thread(&Renderer::renderTile, this, m_currentTile));
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

void Renderer::renderTile(int tileIndex)
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

colRGB Renderer::raycast(Ray ray, float &t)
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
void Renderer::traverseBVH(Ray &ray, bvhNode *node, DifferentialGeometry &dg, bool &hasHit, float minT)
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
bool Renderer::shadowRay(line ln)
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
void Renderer::shadowTraverseBVH(Ray &ray, bvhNode *node, bool &hasHit)
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

colRGB Renderer::shade(DifferentialGeometry dg)
{
	colRGB ret = colRGB(0, 0, 0);
	Shader mat = m_ScenePtr->materials[dg.mat];
	colRGB dif = mat.diffuse;
	if (mat.hasDifTex)
	{
		dif = m_ScenePtr->textures[mat.difTexIdx].getRGB(dg.uv.x, dg.uv.y);
	}
	colRGB spec = mat.specular;
	if (mat.hasSpecTex)
	{
		spec = m_ScenePtr->textures[mat.specTexIdx].getRGB(dg.uv.x, dg.uv.y);
	}
	vec3 N = dg.n;
	if (mat.hasNormTex && dg.hasTangentSpace)
	{
		colRGB texCol = m_ScenePtr->textures[mat.normTexIdx].getRGB(dg.uv.x, dg.uv.y);
		vec3 texN;
		texN.x = (texCol.red * 2) - 1;
		texN.y = (texCol.green * 2) - 1;
		texN.z = (texCol.blue * 2) - 1;
		vec3 tSpaceN = dg.t*texN.x + dg.b*texN.y + dg.n*texN.z;
		N = tSpaceN.Norm();
	}
	switch (mat.shade)
	{
		case DIFFUSE:
		{
			colRGB difComp = colRGB(0, 0, 0);
			for (size_t i = 0; i < m_ScenePtr->lights.size(); i++)
			{
				//L inncoming light direction
				vec3 L = (m_ScenePtr->lights[i]->getPosition() - dg.i.p).Norm(0.00001f);
				float LightIntensity = getLightIntensity(m_ScenePtr->lights[i], dg.i.p);
				if (LightIntensity>0)
				{
					float intensity = N.Dot(L) * LightIntensity;
					if (intensity > 0)
					{
						difComp += dif*m_ScenePtr->lights[i]->getColor()*intensity;
					}
				}
			}
			ret = difComp;
		}
		break;
		case REFLECT:
		{
			vec3 V = dg.dir.Norm(0.0001f);
			colRGB acc = colRGB(0.f, 0.f, 0.f);
			for (size_t i = 0; i < m_ScenePtr->lights.size(); i++)
			{
				//L inncoming light direction
				vec3 L = (m_ScenePtr->lights[i]->getPosition() - dg.i.p).Norm(0.00001f);
				float LightIntensity = getLightIntensity(m_ScenePtr->lights[i], dg.i.p);
				if (LightIntensity>0)
				{
					float intensity = N.Dot(L)*LightIntensity;
					if (intensity > 0)
					{
						acc += dif*m_ScenePtr->lights[i]->getColor()*intensity;
					}
					vec3 R = L - (N*L.Dot(N)) * 2;
					float dot = V.Dot(R);
					if (dot > 0)
					{
						float specI = pow(dot, mat.param.ke)*mat.param.ks*LightIntensity;
						acc += spec*m_ScenePtr->lights[i]->getColor()*specI;
					}
				}
			}
			float refl = mat.param.ks;
			if (refl > 0){
				vec3 R = V - (N*V.Dot(N)) * 2;
				if (dg.bounces > 0)
				{
					Ray ray = Ray(line(dg.i.p, R), dg.bounces - 1, false);
					ray.precalculate();
					float t;
					colRGB rcol = raycast(ray, t);
					acc += rcol*refl*mat.specular;
				}
			}
			ret = acc;
		}
		break;
		case PHONG:
		{
			vec3 V = dg.dir.Norm(0.0001f);
			colRGB acc = colRGB(0.f, 0.f, 0.f);
			for (size_t i = 0; i < m_ScenePtr->lights.size(); i++)
			{
				//L inncoming light direction
				vec3 L = (m_ScenePtr->lights[i]->getPosition() - dg.i.p).Norm(0.00001f);
				float LightIntensity = getLightIntensity(m_ScenePtr->lights[i], dg.i.p);
				if (LightIntensity>0)
				{
					float intensity = N.Dot(L)*LightIntensity;
					if (intensity > 0)
					{
						acc += dif*m_ScenePtr->lights[i]->getColor()*intensity;
					}
					vec3 R = L - (N*L.Dot(N)) * 2;
					float dot = V.Dot(R);
					if (dot > 0)
					{
						float specI = pow(dot, mat.param.ke)*mat.param.ks*LightIntensity;
						acc += spec*m_ScenePtr->lights[i]->getColor()*specI;
					}
				}
			}
			ret = acc;
		}
		break;
		case GLASS:
		{
			vec3 V = dg.dir.Norm(0.0001f);
			colRGB acc = colRGB(0.f, 0.f, 0.f);
			for (size_t i = 0; i < m_ScenePtr->lights.size(); i++)
			{
				//L inncoming light direction
				vec3 L = (m_ScenePtr->lights[i]->getPosition() - dg.i.p).Norm(0.00001f);
				float LightIntensity = getLightIntensity(m_ScenePtr->lights[i], dg.i.p);
				if (LightIntensity>0)
				{
					float intensity = N.Dot(L)*LightIntensity;
					if (intensity > 0)
					{
						acc += dif*m_ScenePtr->lights[i]->getColor()*intensity;
					}
					vec3 R = L - (N*L.Dot(N)) * 2;
					float dot = V.Dot(R);
					if (dot > 0)
					{
						float specI = pow(dot, mat.param.ke)*mat.param.ks*LightIntensity;
						acc += spec*m_ScenePtr->lights[i]->getColor()*specI;
					}
				}
			}
			float refl = mat.param.refl;
			if (refl > 0){
				if (dg.bounces > 0)
				{
					vec3 R = V - (N*V.Dot(N)) * 2;
					Ray ray = Ray(line(dg.i.p, R), dg.bounces - 1, false);
					ray.precalculate();
					float t;
					colRGB rcol = raycast(ray, t);
					acc += rcol*refl*mat.specular;
				}
			}
			float refr = mat.param.refr;
			if (refr > 0.f)
			{
				if (dg.bounces > 0)
				{
					float ior = mat.param.ior;
					float n = 1 / ior;
					float cosI = -(dg.n.Dot(V));
					if (cosI < 0)
						N = -N;
					float cosT2 = 1.f - n*n*(1.f - cosI*cosI);
					if (cosT2 > 0)
					{
						vec3 T = (n*V) + (n*cosI - sqrtf(cosT2)) * N;
						Ray ray = Ray(line(dg.i.p, T), dg.bounces - 1, false);
						ray.precalculate();
						float t;
						colRGB rcol = raycast(ray, t);
						colRGB absorbance = dif*(0.15f*-t);
						colRGB transparency = colRGB(expf(absorbance.red), expf(absorbance.green), expf(absorbance.blue));
						acc += (rcol*transparency);// *refr;
					}

				}
			}
			ret = acc;
		}
			break;
		case GLOSSY:
		case FLAT:
			ret = dif;
			break;
		case EMISSION:
		case BACKGROUND:
			ret = m_ScenePtr->background;
			break;
		case MIX:
			break;
	}
	return ret;
}

float Renderer::getLightIntensity(light *L, point3 P)
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
		float sampleDifferential = (1.0 / (float)((AreaLight*)L)->samplesSq);
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0, sampleDifferential);
		for (float x = 0.f; x < 1.f; x += sampleDifferential)
		{
			for (float y = 0.f; y < 1.f; y += sampleDifferential)
			{
				point3 pos = ((AreaLight*)L)->pos + (x + dis(gen))*((AreaLight*)L)->dir1 + (y+dis(gen))*((AreaLight*)L)->dir2;
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


vector<tile> Renderer::setupTiles(int camWidth, int camHeight, int tileSizeX, int tileSizeY)
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