#pragma once
#include "commonR.hpp"

class TraceUnit;
class Scene;
enum ShadingFunction
{
	SHADER_BACKGROUND,
	SHADER_DIFFUSE_BRDF,
	SHADER_EMISSION_BRDF,
	SHADER_GLASS_BRDF,
	SHADER_GLOSSY_BRDF,
	SHADER_MIX,

	SHADER_UNDEFINED
};

class shader
{
public:
	shader();
	virtual ~shader();
	virtual colRGB shade(DifferentialGeometry dg, Scene *lScPtr, TraceUnit *lRenPtr) = 0;
	virtual ShadingFunction getType();
};

