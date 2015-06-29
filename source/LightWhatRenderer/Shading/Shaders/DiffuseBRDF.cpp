#include "DiffuseBRDF.hpp"
#include "../../Kernel/TraceUnit.hpp"
#include "../../Kernel/LWScene.hpp"


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

colRGB DiffuseBRDF::shade(DifferentialGeometry dg, LWScene *lScPtr, TraceUnit *lRenPtr)
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
	if (N.Dot(dg.dir) > 0)
	{
		N = -N;
	}
	//Actual Shader
	if (dg.bounces > 0)
	{	
		//generate cosine weighted hemisphere vector to surface normal
		vec3 T, B;
		if (dg.hasTangentSpace){T = dg.t, B = dg.b;}
		else MonteCarlo::tangentBitangent(N, T, B);
		vec3 R = MonteCarlo::cosineSampleHemisphere(N, T, B);
		//Recursivly Bounce
		Ray ray = Ray(line(dg.i.p, R), dg.bounces - 1, false);
		ray.precalculate();
		float t;
		colRGB rcol = lRenPtr->raycast(ray, t);

		ret = rcol*dif*intensity;
	}
	else //calculate direct illumination
	{
		for (size_t i = 0; i < lScPtr->lights.size(); i++)
		{
			//L inncoming light direction
			vec3 L = (lScPtr->lights[i]->getPosition() - dg.i.p);
			float distance = L.Length();
			L /= distance;
			float LightIntensity = lRenPtr->getLightIntensity(lScPtr->lights[i], dg.i.p);
			if (LightIntensity>0)
			{
				float r = 1.f;
				float d = max(distance - r, 0);
				float denom = d / r + 1;
				float attenuation = 1 / (denom*denom);
				float intensity = N.Dot(L) * LightIntensity * attenuation;
				if (intensity > 0.f)
				{
					ret += dif*lScPtr->lights[i]->getColor()*intensity;
				}
			}
		}
	}
	return ret;
}

ShadingFunction DiffuseBRDF::getType()
{
	return ShadingFunction::SHADER_DIFFUSE_BRDF;
}