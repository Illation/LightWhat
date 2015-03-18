#pragma once
#include "..\shader.hpp"
class GlossyBRDF :
	public shader
{
public:
	GlossyBRDF();
	GlossyBRDF(colRGB Dif, float DifIntensity, bool HasDifTex, bool DifTexIdx,
		colRGB Spec, float SpecIntensity, bool HasSpecTex, size_t SpecTexIdx,
		float SpecExponent, float ReflIntensity, float ConeAngle);
	virtual ~GlossyBRDF();
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
	float glossMaxR = 0.f;
	float glossMaxDelta = 0.f;

	bool hasNormTex = false;
	size_t normTexIdx;
};

