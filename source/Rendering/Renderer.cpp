#include "Renderer.h"													

Renderer::Renderer()
{
	// nothing to create
}

Renderer::~Renderer()																						
{
}

void Renderer::setScene(Scene *sc){
	m_ScenePtr = sc;
}

void Renderer::init(int camWidth, int camHeight){
	//setup blank image
	double bgCol = 0.6196;
	for (int i = 0; i < camWidth; i++)
	{
		vector<colRGB> colColumn;
		for (int j = 0; j < camHeight; j++)
		{
			colColumn.push_back(colRGB(bgCol, bgCol, bgCol));
		}
		m_Image.push_back(colColumn);
	}

	//setup Ray Map
	rayMap = m_ScenePtr->cam.getRayMap(p_MaxBounces);
}

std::vector  <vector<colRGB> >Renderer::getImage()
{
	return m_Image;
}

bool Renderer::renderNextRow()
{
	vector<Ray> rayColumn = rayMap[m_currentRow];
	vector<colRGB> colColumn;
	for (size_t i = 0; i < rayColumn.size();i++)
	{
		colColumn.push_back(raycast(rayColumn[i]));
	} 
	m_Image[m_currentRow] = colColumn;

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
	double minT = ray.isPrimary ? 1.0 : 0.0001;
	for (size_t i = 0; i < m_ScenePtr->shapes.size(); i++)
	{
		m_ScenePtr->shapes[i]->getIntersection(ray, closest, minT, m_BackfaceCulling);
		if (closest.i.hit)
		{
			hasHit = true;
		}
	}
	closest.dir = ray.ln.dir;
	closest.bounces = ray.bounces;
	if (hasHit == false)
	{
		closest.mat = materialPointer::BACKGROUND_MATERIAL;
	}
	return shade(closest);
}
bool Renderer::shadowRay(line ln)
{
	intersection its;
	bool hasHit = false;
	double shadowLength = ln.dir.Length();
	for (size_t i = 0; i < m_ScenePtr->shapes.size(); i++)
	{
		if (m_ScenePtr->shapes[i]->shadowIntersection(ln))hasHit = true;
	}
	return hasHit;
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
				vec3 L = (m_ScenePtr->lights[i].center - dg.i.p).Norm(0.00001);
				if (!shadowRay(line(dg.i.p, L)))
				{
					double intensity = N.Dot(L);
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
			vec3 V = dg.dir.Norm(0.0001);
			colRGB acc = colRGB(0, 0, 0);
			for (size_t i = 0; i < m_ScenePtr->lights.size(); i++)
			{
				//L inncoming light direction
				vec3 L = (m_ScenePtr->lights[i].center - dg.i.p).Norm(0.00001);
				if (!shadowRay(line(dg.i.p, L)))
				{
					double intensity = N.Dot(L);
					if (intensity > 0)
					{
						acc += dif*m_ScenePtr->lights[i].col*intensity;
					}
					vec3 R = L - (N*L.Dot(N)) * 2;
					double dot = V.Dot(R);
					if (dot > 0)
					{
						double specI = pow(dot, mat.param.ke)*mat.param.ks;
						acc += spec*m_ScenePtr->lights[i].col*specI;
					}
				}
			}
			double refl = mat.param.ks;
			if (refl > 0){
				vec3 R = V - (N*V.Dot(N)) * 2;
				if (dg.bounces > 0)
				{
					Ray ray = Ray(line(dg.i.p, R), dg.bounces - 1, false);
					colRGB rcol = raycast(ray);
					acc += rcol*refl*mat.specular;
				}
			}
			ret = acc;
		}
		break;
		case PHONG:
		{
			vec3 V = dg.dir.Norm(0.0001);
			colRGB acc = colRGB(0, 0, 0);
			for (size_t i = 0; i < m_ScenePtr->lights.size(); i++)
			{
				//L inncoming light direction
				vec3 L = (m_ScenePtr->lights[i].center - dg.i.p).Norm(0.00001);
				if (!shadowRay(line(dg.i.p, L)))
				{
					double intensity = N.Dot(L);
					if (intensity > 0)
					{
						acc += dif*m_ScenePtr->lights[i].col*intensity;
					}
					vec3 R = L - (N*L.Dot(N)) * 2;
					double dot = V.Dot(R);
					if (dot > 0)
					{
						double specI = pow(dot, mat.param.ke)*mat.param.ks;
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