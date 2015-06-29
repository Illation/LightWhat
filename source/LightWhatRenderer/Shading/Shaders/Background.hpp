#pragma once
#include "..\shader.hpp"
class Background :
	public shader
{
public:
	Background();
	virtual ~Background();
	colRGB shade(DifferentialGeometry dg, LWScene *lScPtr, TraceUnit *lRenPtr);
	ShadingFunction getType();
};

