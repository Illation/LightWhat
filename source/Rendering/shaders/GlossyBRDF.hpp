#pragma once
#include "..\shader.hpp"
class GlossyBRDF :
	public shader
{
public:
	GlossyBRDF();
	GlossyBRDF(colRGB Col,  bool HasTex, bool TexIdx, float ReflIntensity, float ConeAngle);
	virtual ~GlossyBRDF();
	colRGB shade(DifferentialGeometry dg, Scene *lScPtr, TraceUnit *lRenPtr);
	ShadingFunction getType();

	colRGB col;
	bool hasTex = false;
	size_t texIdx;

	float reflIntensity = 1.f;
	float glossAngle = 0.f;

	bool hasNormTex = false;
	size_t normTexIdx;
};

