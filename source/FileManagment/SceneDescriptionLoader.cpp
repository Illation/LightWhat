#include "SceneDescriptionLoader.hpp"


SceneDescriptionLoader::SceneDescriptionLoader()
{
}


SceneDescriptionLoader::~SceneDescriptionLoader()
{
}

void SceneDescriptionLoader::loadSceneDescriptionFile(const char *filename)
{
	XMLParser parse;
	XMLdom *dom = new XMLdom;
	parse.loadDocument(filename);
	parse.parse(dom);

	vector<XMLelement*>settingsElementList = dom->root->getElementsCalled(std::wstring(L"renderSettings"));
	if (settingsElementList.size() > 0)
	{
		settings = getSettings(settingsElementList[0]);
	}
	vector<XMLelement*>cameraElementList = dom->root->getElementsCalled(std::wstring(L"camera"));
	if (cameraElementList.size() > 0)
	{
		cam = getCamera(cameraElementList[0]);
	}
}

renderSettings SceneDescriptionLoader::getSettings(XMLelement *node)
{
	renderSettings ret;
	for (size_t i = 0; i < node->children.size(); i++)
	{
		if (node->children[i]->name == std::wstring(L"resolution"))//get resolution
		{
			for (size_t j = 0; j < node->children[i]->attributes.size(); j++)
			{
				if (node->children[i]->attributes[j].name == std::wstring(L"x"))
				{
					ret.resolutionX = node->children[i]->attributes[j].getInt();
				}
				if (node->children[i]->attributes[j].name == std::wstring(L"y"))
				{
					ret.resolutionY = node->children[i]->attributes[j].getInt();
				}
			}
		}
		if (node->children[i]->name == std::wstring(L"samples"))//get samples
		{
			ret.samples = node->children[i]->getInt();
		}
		if (node->children[i]->name == std::wstring(L"bounces"))//get bounces
		{
			ret.bounces = node->children[i]->getInt();
		}
		if (node->children[i]->name == std::wstring(L"backfaceCulling"))//get bfc
		{
			if(node->children[i]->content == std::wstring(L"true"))ret.backfaceCulling=true;
			else ret.backfaceCulling = false;
		}
		if (node->children[i]->name == std::wstring(L"toneMapping"))//get toneMapping
		{
			if (node->children[i]->content == std::wstring(L"CLIP"))ret.toneMapping = CLIP;
			else ret.toneMapping = AUTO;
		}
		if (node->children[i]->name == std::wstring(L"gammaEnabled"))//get gammaEnabled
		{
			if (node->children[i]->content == std::wstring(L"true"))ret.gammaEnabled = true;
			else ret.gammaEnabled = false;
		}
		if (node->children[i]->name == std::wstring(L"gammaExponent"))//get gammaExponent
		{
			ret.gammaExponent = node->children[i]->getFloat();
		}
		if (node->children[i]->name == std::wstring(L"background"))//get background
		{
			for (size_t j = 0; j < node->children[i]->attributes.size(); j++)
			{
				if (node->children[i]->attributes[j].name == std::wstring(L"r"))
				{
					ret.background.red = node->children[i]->attributes[j].getFloat();
				}
				if (node->children[i]->attributes[j].name == std::wstring(L"g"))
				{
					ret.background.green = node->children[i]->attributes[j].getFloat();
				}
				if (node->children[i]->attributes[j].name == std::wstring(L"b"))
				{
					ret.background.blue = node->children[i]->attributes[j].getFloat();
				}
			}
		}
	}
	return ret;
}

Camera SceneDescriptionLoader::getCamera(XMLelement *node)
{
	Camera ret;
	vec3 position;
	vec3 direction;
	bool dirIsT = false;
	float angle = 50.f;
	float nCP = 1.f;
	float fCP = 100.f;
	for (size_t i = 0; i < node->children.size(); i++)
	{
		if (node->children[i]->name == std::wstring(L"position"))//get position
		{
			position = getAttributeVector(node->children[i]);
		}
		if (node->children[i]->name == std::wstring(L"direction"))//get direction
		{
			direction = getAttributeVector(node->children[i]);
		}
		if (node->children[i]->name == std::wstring(L"directionIsTarget"))//set target?
		{
			if (node->children[i]->content == std::wstring(L"true"))dirIsT = true;
			else dirIsT = false;
		}
		if (node->children[i]->name == std::wstring(L"angle"))//get bfc
		{
			angle = node->children[i]->getFloat();
		}
		if (node->children[i]->name == std::wstring(L"clipping"))//get clipping planes
		{
			for (size_t j = 0; j < node->children[i]->attributes.size(); j++)
			{
				if (node->children[i]->attributes[j].name == std::wstring(L"near"))
				{
					nCP = node->children[i]->attributes[j].getFloat();
				}
				if (node->children[i]->attributes[j].name == std::wstring(L"far"))
				{
					fCP = node->children[i]->attributes[j].getFloat();
				}
			}
		}
	}
	ret = Camera(position, direction, settings.resolutionX, settings.resolutionY);
	if (dirIsT)
		ret.setTarget(direction);
	ret.setParameters(angle, nCP, fCP);
	return ret;
}

vec3 SceneDescriptionLoader::getAttributeVector(XMLelement *node)
{
	vec3 ret;
	for (size_t j = 0; j < node->attributes.size(); j++)
	{
		if (node->attributes[j].name == std::wstring(L"x"))
		{
			ret.x = node->attributes[j].getFloat();
		}
		if (node->attributes[j].name == std::wstring(L"y"))
		{
			ret.y = node->attributes[j].getFloat();
		}
		if (node->attributes[j].name == std::wstring(L"z"))
		{
			ret.z = node->attributes[j].getFloat();
		}
	}
	return ret;
}