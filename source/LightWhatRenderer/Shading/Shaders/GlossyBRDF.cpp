#include "GlossyBRDF.hpp"
#include "../../Kernel/TraceUnit.hpp"
#include "../../Kernel/LWScene.hpp"


GlossyBRDF::GlossyBRDF()
{
}
GlossyBRDF::GlossyBRDF(colRGB Col, bool HasTex, bool TexIdx, float ReflIntensity, float ConeAngle)
{
	col = Col;
	hasTex = HasTex;
	texIdx = TexIdx;
	reflIntensity = ReflIntensity;
	glossAngle = ConeAngle * (PI / 180.f);
}
GlossyBRDF::~GlossyBRDF()
{
}

colRGB GlossyBRDF::shade(DifferentialGeometry dg, LWScene *lScPtr, TraceUnit *lRenPtr)
{
	colRGB ret = colRGB(0, 0, 0);
	if (reflIntensity > 0){
		//Header
		colRGB c = col;
		if (hasTex)
		{
			c = lScPtr->textures[texIdx].getRGB(dg.uv.x, dg.uv.y);
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
		vec3 V = dg.dir.Norm(0.0001f);
		vec3 R = V - (N*V.Dot(N)) * 2;
		if (dg.bounces > 0)
		{
			if (glossAngle > 0)
			{
				vec3 T, B;
				if (dg.hasTangentSpace){ T = dg.t, B = dg.b; }
				else MonteCarlo::tangentBitangent(R, T, B);
				R = MonteCarlo::uniformSampleCone(R, T, B, glossAngle);
			}
			Ray ray = Ray(line(dg.i.p, R), dg.bounces - 1, false);
			ray.precalculate();
			float t;
			colRGB rcol = lRenPtr->raycast(ray, t);
			ret += rcol*reflIntensity*c;
		}
		else //calculate direct illumination
		{
			for (size_t i = 0; i < lScPtr->lights.size(); i++)
			{
				//L inncoming light direction
				vec3 L = (lScPtr->lights[i]->getPosition() - dg.i.p).Norm(0.00001f);
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
					float dot = L.Dot(R);
					if (dot > 0)
					{
						float specI = powf(dot, ((0.5f*PI) - glossAngle)*28.6478897565f)*reflIntensity*LightIntensity;
						ret += c*lScPtr->lights[i]->getColor()*specI;
					}
				}
			}
		}
	}
	return ret;
}

ShadingFunction GlossyBRDF::getType()
{
	return ShadingFunction::SHADER_GLOSSY_BRDF;
}