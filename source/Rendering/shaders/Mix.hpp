#pragma once
#include "..\shader.hpp"
class Mix :
	public shader
{
public:
	Mix();
	Mix(shader *Shader1, shader *Shader2, float Factor);
	virtual ~Mix();
	colRGB shade(DifferentialGeometry dg, Scene *lScPtr, TraceUnit *lRenPtr);
	ShadingFunction getType();

	shader *shade1Ptr = nullptr, *shade2Ptr = nullptr;
	float factor = 0.5;
};

