#pragma once
#include "..\shader.hpp"
class DiffuseBRDF :
	public shader
{
public:
	DiffuseBRDF();
	DiffuseBRDF(colRGB, float Intensity, bool hasTexture, size_t texIdx);
	virtual ~DiffuseBRDF();
	colRGB shade(DifferentialGeometry dg, Scene *lScPtr, Renderer *lRenPtr);
	ShadingFunction getType();

	colRGB diffuse;
	bool hasTexture = false;
	size_t TexIdx;
	float intensity;

	bool hasNormTex = false;
	size_t normTexIdx;
};

