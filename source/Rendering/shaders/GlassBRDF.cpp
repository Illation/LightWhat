#include "GlassBRDF.hpp"
#include "../TraceUnit.hpp"
#include "../Scene.hpp"


GlassBRDF::GlassBRDF()
{
}
GlassBRDF::GlassBRDF(colRGB Col, bool HasTex, size_t TexIdx, float ReflRatio, float IOR, float RoughAngle)
{
	col = Col;
	hasTexture = HasTex;
	texIdx = TexIdx;
	reflRatio = ReflRatio;
	ior = IOR;
	roughAngle = RoughAngle;
}

GlassBRDF::~GlassBRDF()
{
}

colRGB GlassBRDF::shade(DifferentialGeometry dg, Scene *lScPtr, TraceUnit *lRenPtr)
{
	colRGB ret = colRGB(0, 0, 0);
	if (dg.bounces > 0)
	{
		//Header
		colRGB c = col;
		if (hasTexture)
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
		vec3 V = dg.dir.Norm(0.0001f);
		//Shader
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0, 1);
		if ((float) dis(gen) < reflRatio){
			vec3 R = V - (N*V.Dot(N)) * 2;
			if (roughAngle > 0)
			{
				vec3 T, B;
				if (dg.hasTangentSpace){ T = dg.t, B = dg.b; }
				else MonteCarlo::tangentBitangent(R, T, B);
				R = MonteCarlo::uniformSampleCone(R, T, B, roughAngle);
			}
			Ray ray = Ray(line(dg.i.p, R), dg.bounces - 1, false);
			ray.precalculate();
			float t;
			colRGB rcol = lRenPtr->raycast(ray, t);
			ret += rcol*c;
		}
		else
		{
			N = (!dg.i.backfacing ? N : -N);
			float n = dg.i.backfacing ? (1.f / ior) : (ior / 1.f);
			float cosI = ((-N).Dot(V));
			float sinT2 = -1 + n*n*(1.f - (cosI*cosI));
			if (sinT2 <= 1.0f)
			{
				vec3 R = (n*V) - (n + cosI - sqrtf(1.f - sinT2)) * N;
				if (roughAngle > 0)
				{
					vec3 T, B;
					if (dg.hasTangentSpace){ T = dg.t, B = dg.b; }
					else MonteCarlo::tangentBitangent(R, T, B);
					R = MonteCarlo::uniformSampleCone(R, T, B, roughAngle);
				}
				Ray ray = Ray(line(dg.i.p, R), dg.bounces - 1, false);
				ray.precalculate();
				float t;
				colRGB rcol = lRenPtr->raycast(ray, t);
				ret = rcol*c;
			}
			else ret += c;
		}
	}
	return ret;
}

ShadingFunction GlassBRDF::getType()
{
	return ShadingFunction::SHADER_GLASS_BRDF;
}