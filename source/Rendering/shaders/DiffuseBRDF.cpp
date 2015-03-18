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
	for (size_t i = 0; i < lScPtr->lights.size(); i++)
	{
		//L inncoming light direction
		vec3 L = (lScPtr->lights[i]->getPosition() - dg.i.p).Norm(0.00001f);
		float LightIntensity = lRenPtr->getLightIntensity(lScPtr->lights[i], dg.i.p);
		if (LightIntensity>0)
		{
			float intensity = N.Dot(L) * LightIntensity;
			if (intensity > 0)
			{
				ret += dif*lScPtr->lights[i]->getColor()*intensity;
			}
		}
	}
	return ret;
}

ShadingFunction DiffuseBRDF::getType()
{
	return ShadingFunction::SHADER_DIFFUSE_BRDF;
}