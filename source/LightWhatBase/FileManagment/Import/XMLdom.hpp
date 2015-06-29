//XML structure by Robert Lindner, 1DAE3
#pragma once
#include<vector>
#include <string>

struct XMLelemnt;
struct XMLattribute;

struct XMLelement{
	double getDouble();
	float getFloat();
	int getInt();

	std::vector<XMLelement*> getElementsCalled(std::wstring name);
	std::vector<XMLattribute> getAttributesCalled(std::wstring name);

	bool isLeaf;
	std::wstring name;
	std::wstring content;
	std::vector<XMLattribute>attributes;
	std::vector<XMLelement*>children;
};
struct XMLattribute{
	double getDouble();
	float getFloat();
	int getInt();

	std::wstring name;
	std::wstring content;
};

class XMLdom
{
public:
	XMLdom();
	~XMLdom();

	std::vector<XMLattribute>attributes;
	XMLelement *root = nullptr;
};

