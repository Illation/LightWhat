#include "Background.hpp"
#include "../TraceUnit.hpp"
#include "../Scene.hpp"


Background::Background()
{
}


Background::~Background()
{
}

colRGB Background::shade(DifferentialGeometry dg, Scene *lScPtr, TraceUnit *lRenPtr)
{
	return lScPtr->background;
}

ShadingFunction Background::getType()
{
	return ShadingFunction::SHADER_BACKGROUND;
}