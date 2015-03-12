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
	m_Image = Texture("Render result", camWidth, camHeight);
	m_Image.setInterpolationMode(INTPOL_PIXELS);
	m_Image.setQuadraticFittingMode(FIT_STRETCHXY);

	//setup Ray Map
	rayMap = m_ScenePtr->cam.getRayMap(p_MaxBounces);

	//setup BVH
	cout << "building bvh...." << endl;
	m_BvhPtr = new BVH();
	m_BvhPtr->Build(m_ScenePtr);
	cout << "bvh built!" << endl;
}

Texture *Renderer::getImage()
{
	return &m_Image;
}

bool Renderer::renderNextRow()
{
	vector<Ray> rayColumn = rayMap[m_currentRow];
	for (size_t i = 0; i < rayColumn.size();i++)
	{
		m_Image.setRGB(raycast(rayColumn[i]), m_currentRow, i);
	} 

	m_currentRow++;
	if (m_currentRow >= m_ScenePtr->cam.screenX)
	{
		m_currentRow = 0;
		m_samplesRendered++;
		return true;
	}
	else return false;
}

colRGB Renderer::raycast(Ray ray)
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
	}
}
bool Renderer::shadowRay(line ln)
{
	bool hasHit = false;
	for (size_t i = 0; i < m_BvhPtr->unsortedIndices.size(); i++)
	{
		shapeNodeIdx idxStruct = m_BvhPtr->unsortedIndices[i];
		if (m_ScenePtr->shapes[idxStruct.shapeIdx]->shadowIntersection(idxStruct.subIdx1, idxStruct.subIdx2, ln))
			hasHit = true;
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
			if (m_ScenePtr->shapes[idxStruct.shapeIdx]->shadowIntersection(idxStruct.subIdx1, idxStruct.subIdx2, ray.ln))
				hasHit = true;
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
				vec3 L = (m_ScenePtr->lights[i].center - dg.i.p).Norm(0.00001f);
				if (!shadowRay(line(dg.i.p, L)))
				{
					float intensity = N.Dot(L);
					if (intensity > 0)
					{
						difComp += dif*m_ScenePtr->lights[i].col*intensity;
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
				vec3 L = (m_ScenePtr->lights[i].center - dg.i.p).Norm(0.00001f);
				if (!shadowRay(line(dg.i.p, L)))
				{
					float intensity = N.Dot(L);
					if (intensity > 0)
					{
						acc += dif*m_ScenePtr->lights[i].col*intensity;
					}
					vec3 R = L - (N*L.Dot(N)) * 2;
					float dot = V.Dot(R);
					if (dot > 0)
					{
						float specI = pow(dot, mat.param.ke)*mat.param.ks;
						acc += spec*m_ScenePtr->lights[i].col*specI;
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
					colRGB rcol = raycast(ray);
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
				vec3 L = (m_ScenePtr->lights[i].center - dg.i.p).Norm(0.00001f);
				if (!shadowRay(line(dg.i.p, L)))
				{
					float intensity = N.Dot(L);
					if (intensity > 0)
					{
						acc += dif*m_ScenePtr->lights[i].col*intensity;
					}
					vec3 R = L - (N*L.Dot(N)) * 2;
					float dot = V.Dot(R);
					if (dot > 0)
					{
						float specI = pow(dot, mat.param.ke)*mat.param.ks;
						acc += spec*m_ScenePtr->lights[i].col*specI;
					}
				}
			}
			ret = acc;
		}
		break;
		case GLOSSY:
		case EMISSION:
		case BACKGROUND:
			ret = m_ScenePtr->background;
			break;
		case MIX:
			break;
	}
	return ret;
}