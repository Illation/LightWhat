#include "GlossyBRDF.hpp"
#include "../Renderer.hpp"
#include "../Scene.hpp"


GlossyBRDF::GlossyBRDF()
{
}
GlossyBRDF::GlossyBRDF(colRGB Dif, float DifIntensity, bool HasDifTex, bool DifTexIdx,
	colRGB Spec, float SpecIntensity, bool HasSpecTex, size_t SpecTexIdx,
	float SpecExponent, float ReflIntensity, float ConeAngle)
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
	float angle = ConeAngle * (PI / 180.f);
	glossMaxR = 1 - cosf(angle);
	glossMaxDelta = sinf(angle);
}
GlossyBRDF::~GlossyBRDF()
{
}

colRGB GlossyBRDF::shade(DifferentialGeometry dg, Scene *lScPtr, Renderer *lRenPtr)
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
	for (size_t i = 0; i < lScPtr->lights.size(); i++)
	{
		//L inncoming light direction
		vec3 L = (lScPtr->lights[i]->getPosition() - dg.i.p).Norm(0.00001f);
		float LightIntensity = lRenPtr->getLightIntensity(lScPtr->lights[i], dg.i.p);
		if (LightIntensity>0)
		{
			float intensity = N.Dot(L)*LightIntensity;
			if (intensity > 0)
			{
				acc += dif*lScPtr->lights[i]->getColor()*intensity;
			}
			vec3 R = L - (N*L.Dot(N)) * 2;
			float dot = V.Dot(R);
			if (dot > 0)
			{
				float specI = pow(dot, specExponent)*specIntensity*LightIntensity;
				acc += spec*lScPtr->lights[i]->getColor()*specI;
			}
		}
	}
	if (reflIntensity > 0){
		if (dg.bounces > 0)
		{
			vec3 R = V - (N*V.Dot(N)) * 2;
			if (glossMaxDelta > 0)
			{
				vec3 RU;
				if (R.x || R.z)RU = vec3(0, 1, 0);
				else RU = vec3(0, 0, 1);
				vec3 RN = R.Cross(RU).Norm();
				vec3 RB = R.Cross(RN).Norm();
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<> dis(-1, 1);
				std::uniform_real_distribution<> disDelta(0, glossMaxDelta);
				std::uniform_real_distribution<> disR(0, glossMaxR);
				vec3 rDelta = (dis(gen)*RN + dis(gen)*RB).Norm()*disDelta(gen);
				R = ((R.Norm())*(1 - disR(gen))) + rDelta;
			}
			Ray ray = Ray(line(dg.i.p, R), dg.bounces - 1, false);
			ray.precalculate();
			float t;
			colRGB rcol = lRenPtr->raycast(ray, t);
			acc += rcol*reflIntensity*specular;
		}
	}
	ret = acc;

	return ret;
}

ShadingFunction GlossyBRDF::getType()
{
	return ShadingFunction::SHADER_GLOSSY_BRDF;
}