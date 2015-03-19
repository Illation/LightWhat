#include "DiffuseBRDF.hpp"
#include "../Renderer.hpp"
#include "../Scene.hpp"


DiffuseBRDF::DiffuseBRDF()
{
}


DiffuseBRDF::~DiffuseBRDF()
{
}

DiffuseBRDF::DiffuseBRDF(colRGB col, float Intensity, bool lHasTexture, size_t texIdx)
{
	diffuse = col;
	intensity = Intensity;
	hasTexture = lHasTexture;
	TexIdx = texIdx;
}

colRGB DiffuseBRDF::shade(DifferentialGeometry dg, Scene *lScPtr, Renderer *lRenPtr)
{
	//Header
	colRGB ret = colRGB(0,0,0);
	colRGB dif = diffuse;
	if (hasTexture)
	{
		dif = lScPtr->textures[TexIdx].getRGB(dg.uv.x, dg.uv.y);
	}
	vec3 N = dg.n;
	if (hasNormTex && dg.hasTangentSpace)
	{
		colRGB texCol = lScPtr->textures[normTexIdx].getRGB(dg.uv.x, dg.uv.y);
		vec3 texN;
		texN.x = (texCol.red * 2) - 1;
		texN.y = (texCol.green * 2) - 1;
		texN.z = (texCol.blue * 2) - 1;
		vec3 tSpaceN = dg.t*texN.x + dg.b*texN.y + N*texN.z;
		N = tSpaceN.Norm();
	}
	//Actual Shader
	if (dg.bounces > 0)
	{	
		//generate random hemisphere vector to surface normal
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(-1, 1);
		vec3 R = vec3(dis(gen), dis(gen), dis(gen)).Norm();
		float cosT = N.Dot(R);
		if (cosT < 0)
		{
			R = -R;
			cosT = -cosT;
		}
		//Recursivly Bounce
		Ray ray = Ray(line(dg.i.p, R), dg.bounces - 1, false);
		ray.precalculate();
		float t;
		colRGB rcol = lRenPtr->raycast(ray, t);

		ret = rcol*dif*intensity*cosT;
	}
	//else //Ignore Lighs for now
	//{
	//	for (size_t i = 0; i < lScPtr->lights.size(); i++)
	//	{
	//		//L inncoming light direction
	//		vec3 L = (lScPtr->lights[i]->getPosition() - dg.i.p).Norm(0.00001f);
	//		float LightIntensity = lRenPtr->getLightIntensity(lScPtr->lights[i], dg.i.p);
	//		if (LightIntensity>0)
	//		{
	//			float intensity = N.Dot(L) * LightIntensity;
	//			if (intensity > 0)
	//			{
	//				ret += dif*lScPtr->lights[i]->getColor()*intensity;
	//			}
	//		}
	//	}
	//}
	return ret;
}

ShadingFunction DiffuseBRDF::getType()
{
	return ShadingFunction::SHADER_DIFFUSE_BRDF;
}