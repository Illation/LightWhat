#pragma once
#include "..\shader.hpp"
class GlassBRDF :
	public shader
{
public:
	GlassBRDF();
	GlassBRDF(colRGB Col, bool HasTex, size_t TexIdx, float ReflRatio, float IOR, float RoughAngle);
	virtual ~GlassBRDF();
	colRGB shade(DifferentialGeometry dg, LWScene *lScPtr, TraceUnit *lRenPtr);
	ShadingFunction getType();

	colRGB col;
	bool hasTexture = false;
	size_t texIdx;
	float reflRatio;
	float ior = 1.3f;
	float roughAngle = 0.f;

	bool hasNormTex = false;
	size_t normTexIdx;
};

