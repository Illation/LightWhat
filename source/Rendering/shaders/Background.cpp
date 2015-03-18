#include "Background.hpp"
#include "../Renderer.hpp"
#include "../Scene.hpp"


Background::Background()
{
}


Background::~Background()
{
}

colRGB Background::shade(DifferentialGeometry dg, Scene *lScPtr, Renderer *lRenPtr)
{
	return lScPtr->background;
}

ShadingFunction Background::getType()
{
	return ShadingFunction::SHADER_BACKGROUND;
}