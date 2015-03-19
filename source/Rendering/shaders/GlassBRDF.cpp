#include "GlassBRDF.hpp"
#include "../Renderer.hpp"
#include "../Scene.hpp"


GlassBRDF::GlassBRDF()
{
}
GlassBRDF::GlassBRDF(colRGB Dif, float DifIntensity, bool HasDifTex, bool DifTexIdx,
	colRGB Spec, float SpecIntensity, bool HasSpecTex, size_t SpecTexIdx,
	float SpecExponent, float ReflIntensity, float RefrIntensity, float IOR)
{
	diffuse = Dif;
	difIntensity = DifIntensity;
	hasDifTexture = HasDifTex;
	difTexIdx = DifTexIdx;
	specular = Spec;
	specIntensity = SpecIntensity;
	hasSpecTex = HasSpecTex;
	specTexIdx = SpecTexIdx;
	specExponent = SpecExponent;
	reflIntensity = ReflIntensity;
	ior = IOR;
	refrIntensity = RefrIntensity;
}

GlassBRDF::~GlassBRDF()
{
}

colRGB GlassBRDF::shade(DifferentialGeometry dg, Scene *lScPtr, Renderer *lRenPtr)
{
	colRGB ret = colRGB(0, 0, 0);
	colRGB dif = diffuse;
	if (hasDifTexture)
	{
		dif = lScPtr->textures[difTexIdx].getRGB(dg.uv.x, dg.uv.y);
	}
	colRGB spec = specular;
	if (hasSpecTex)
	{
		spec = lScPtr->textures[specTexIdx].getRGB(dg.uv.x, dg.uv.y);
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

	vec3 V = dg.dir.Norm(0.0001f);
	colRGB acc = colRGB(0.f, 0.f, 0.f);
	//for (size_t i = 0; i < lScPtr->lights.size(); i++)
	//{
	//	//L inncoming light direction
	//	vec3 L = (lScPtr->lights[i]->getPosition() - dg.i.p).Norm(0.00001f);
	//	float LightIntensity = lRenPtr->getLightIntensity(lScPtr->lights[i], dg.i.p);
	//	if (LightIntensity>0)
	//	{
	//		float intensity = N.Dot(L)*LightIntensity;
	//		if (intensity > 0)
	//		{
	//			acc += dif*lScPtr->lights[i]->getColor()*intensity;
	//		}
	//		vec3 R = L - (N*L.Dot(N)) * 2;
	//		float dot = V.Dot(R);
	//		if (dot > 0)
	//		{
	//			float specI = pow(dot, specExponent)*specIntensity*LightIntensity;
	//			acc += spec*lScPtr->lights[i]->getColor()*specI;
	//		}
	//	}
	//}
	if (reflIntensity > 0){
		if (dg.bounces > 0)
		{
			vec3 R = V - (N*V.Dot(N)) * 2;
			Ray ray = Ray(line(dg.i.p, R), dg.bounces - 1, false);
			ray.precalculate();
			float t;
			colRGB rcol = lRenPtr->raycast(ray, t);
			acc += rcol*reflIntensity*specular;
		}
	}
	if (refrIntensity > 0.f)
	{
		if (dg.bounces > 0)
		{
			float n = 1 / ior;
			float cosI = -(N.Dot(V));
			if (cosI < 0)
				N = -N;
			float cosT2 = 1.f - n*n*(1.f - cosI*cosI);
			if (cosT2 > 0)
			{
				vec3 T = (n*V) + (n*cosI - sqrtf(cosT2)) * N;
				Ray ray = Ray(line(dg.i.p, T), dg.bounces - 1, false);
				ray.precalculate();
				float t;
				colRGB rcol = lRenPtr->raycast(ray, t);
				colRGB absorbance = dif*(0.15f*-t);
				colRGB transparency = colRGB(expf(absorbance.red), expf(absorbance.green), expf(absorbance.blue));
				acc += (rcol*transparency);// *refr;
			}

		}
	}
	ret = acc/(reflIntensity+refrIntensity);
	return ret;
}

ShadingFunction GlassBRDF::getType()
{
	return ShadingFunction::SHADER_GLASS_BRDF;
}