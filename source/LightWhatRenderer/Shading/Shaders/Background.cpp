#include "Background.hpp"
#include "../../Kernel/TraceUnit.hpp"
#include "../../Kernel/LWScene.hpp"


Background::Background()
{
}


Background::~Background()
{
}

colRGB Background::shade(DifferentialGeometry dg, LWScene *lScPtr, TraceUnit *lRenPtr)
{
	return lScPtr->background;
}

ShadingFunction Background::getType()
{
	return ShadingFunction::SHADER_BACKGROUND;
}