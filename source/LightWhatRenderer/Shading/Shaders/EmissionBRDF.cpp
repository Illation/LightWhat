#include "EmissionBRDF.hpp"
#include "../../Kernel/TraceUnit.hpp"
#include "../../Kernel/LWScene.hpp"


EmissionBRDF::EmissionBRDF()
{
}
EmissionBRDF::EmissionBRDF(colRGB Emission, float intensity, bool HasTexture, size_t TexIdx)
{
	emission = Emission;
	Intensity = intensity;
	hasTexture = HasTexture;
	texIdx = TexIdx;
}
EmissionBRDF::~EmissionBRDF()
{
}

colRGB EmissionBRDF::shade(DifferentialGeometry dg, LWScene *lScPtr, TraceUnit *lRenPtr)
{
	colRGB col = emission;
	if (hasTexture)
	{
		col = lScPtr->textures[texIdx].getRGB(dg.uv.x, dg.uv.y);
	}
	return col*Intensity;
}

ShadingFunction EmissionBRDF::getType()
{
	return ShadingFunction::SHADER_EMISSION_BRDF;
}