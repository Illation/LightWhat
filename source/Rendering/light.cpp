#include "light.hpp"


light::light()
{
}


light::~light()
{
}

point3 light::getPosition(){
	cout << "unimplemented method: getPosition() called!" << endl;
	return point3(0, 0, 0);
}

LightType light::getType(){
	cout << "unimplemented method: getType() called!" << endl;
	return LightType::LIGHT_UNDEFINED;
}

float light::getIntensity(){
	cout << "unimplemented method: getIntensity() called!" << endl;
	return 0.f;
}

colRGB light::getColor(){
	cout << "unimplemented method: getColor() called!" << endl;
	return colRGB(0.f,0.f,0.f);
}