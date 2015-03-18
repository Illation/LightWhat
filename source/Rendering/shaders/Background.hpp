#pragma once
#include "..\shader.hpp"
class Background :
	public shader
{
public:
	Background();
	virtual ~Background();
	colRGB shade(DifferentialGeometry dg, Scene *lScPtr, Renderer *lRenPtr);
	ShadingFunction getType();
};

