#pragma once
#include "..\shader.hpp"
class GlassBRDF :
	public shader
{
public:
	GlassBRDF();
	GlassBRDF(colRGB Dif, float DifIntensity, bool HasDifTex, bool DifTexIdx,
		colRGB Spec, float SpecIntensity, bool HasSpecTex, size_t SpecTexIdx,
		float SpecExponent, float ReflIntensity, float RefrIntensity, float IOR);
	virtual ~GlassBRDF();
	colRGB shade(DifferentialGeometry dg, Scene *lScPtr, Renderer *lRenPtr);
	ShadingFunction getType();

	colRGB diffuse;
	bool hasDifTexture = false;
	size_t difTexIdx;
	float difIntensity;

	colRGB specular;
	bool hasSpecTex = false;
	size_t specTexIdx;
	float specIntensity;
	float specExponent;

	float reflIntensity = 1.f;

	float ior = 1.3f;
	float refrIntensity = 0.f;

	bool hasNormTex = false;
	size_t normTexIdx;
};

