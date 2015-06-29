#pragma once
#include "../shader.hpp"
#include "../../Utility/MonteCarlo.hpp"
class DiffuseBRDF :
	public shader
{
public:
	DiffuseBRDF();
	DiffuseBRDF(colRGB, float Intensity, bool hasTexture, size_t texIdx);
	virtual ~DiffuseBRDF();
	colRGB shade(DifferentialGeometry dg, LWScene *lScPtr, TraceUnit *lRenPtr);
	ShadingFunction getType();

	colRGB diffuse;
	bool hasTexture = false;
	size_t TexIdx;
	float intensity;

	bool hasNormTex = false;
	size_t normTexIdx;
private:
	//vec3 cosineSampleHemisphere(float u1, float u2);
};

