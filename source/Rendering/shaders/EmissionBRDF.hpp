#pragma once
#include "..\shader.hpp"
class EmissionBRDF :
	public shader
{
public:
	EmissionBRDF();
	EmissionBRDF(colRGB Emission, float intensity, bool hasTexture, size_t TexIdx);
	virtual ~EmissionBRDF();
	colRGB shade(DifferentialGeometry dg, Scene *lScPtr, Renderer *lRenPtr);
	ShadingFunction getType();

	colRGB emission;
	float Intensity;
	bool hasTexture = false;;
	size_t texIdx;
};

