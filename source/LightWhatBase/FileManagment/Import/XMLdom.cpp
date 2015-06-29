#include "XMLdom.hpp"

double XMLelement::getDouble()
{
	return stod(content);
}
float XMLelement::getFloat()
{
	return stof(content);
}
int XMLelement::getInt()
{
	return stoi(content);
}
std::vector<XMLelement*> XMLelement::getElementsCalled(std::wstring name)
{
	using namespace std;
	vector<XMLelement*> ret;
	for (size_t i = 0; i < children.size(); i++)
	{
		if (name == children[i]->name)
			ret.push_back(children[i]);
	}
	return ret;
}
std::vector<XMLattribute> XMLelement::getAttributesCalled(std::wstring name)
{
	using namespace std;
	vector<XMLattribute> ret;
	for (size_t i = 0; i < attributes.size(); i++)
	{
		if (name == attributes[i].name)
			ret.push_back(attributes[i]);
	}
	return ret;
}

double XMLattribute::getDouble()
{
	return stod(content);
}
float XMLattribute::getFloat()
{
	return stof(content);
}
int XMLattribute::getInt()
{
	return stoi(content);
}

XMLdom::XMLdom()
{
}
XMLdom::~XMLdom()
{
}