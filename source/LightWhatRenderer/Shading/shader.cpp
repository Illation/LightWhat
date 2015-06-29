#include "shader.hpp"


shader::shader()
{
}


shader::~shader()
{
}

colRGB shader::shade(DifferentialGeometry dg, LWScene *lScPtr, TraceUnit *lRenPtr)
{
	cout << "unimplemented method: shade() called!" << endl;
	return colRGB(0, 0, 0);
}

ShadingFunction shader::getType()
{
	cout << "unimplemented method: getType() called!" << endl;
	return ShadingFunction::SHADER_UNDEFINED;
}
