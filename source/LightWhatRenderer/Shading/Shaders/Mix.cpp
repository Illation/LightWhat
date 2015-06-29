#include "Mix.hpp"
#include "../../Kernel/TraceUnit.hpp"
#include "../../Kernel/LWScene.hpp"


Mix::Mix()
{
}
Mix::Mix(shader *Shader1, shader *Shader2, float Factor)
{
	shade1Ptr = Shader1, shade2Ptr = Shader2;
	factor = Factor;
}
Mix::~Mix()
{
}

colRGB Mix::shade(DifferentialGeometry dg, LWScene *lScPtr, TraceUnit *lRenPtr)
{
	colRGB ret = colRGB(0,0,0);
	colRGB c1 = shade1Ptr->shade(dg, lScPtr, lRenPtr);
	colRGB c2 = shade2Ptr->shade(dg, lScPtr, lRenPtr);
	return (c1*(1 - factor))+(c2*(factor));
}

ShadingFunction Mix::getType()
{
	return ShadingFunction::SHADER_MIX;
}