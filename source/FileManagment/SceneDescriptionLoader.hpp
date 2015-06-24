#pragma once
#include "XMLParser.hpp"
#include "XMLdom.hpp"
#include "../Rendering/PostProcessor.hpp"
#include "../Rendering/Scene.hpp"

struct renderSettings{
	int resolutionX;
	int resolutionY;
	int samples;
	int bounces;
	bool backfaceCulling;
	ExposureType toneMapping;
	bool gammaEnabled;
	float gammaExponent;
	colRGB background;
};
class SceneDescriptionLoader
{
public:
	SceneDescriptionLoader();
	~SceneDescriptionLoader();

	void loadSceneDescriptionFile(const char *filename);

	renderSettings settings;
	Camera cam;
	vector  <shape*>shapes;
	vector  <light*>lights;
	vector  <shader*>materials;
	vector  <Texture>textures;

private:
	renderSettings getSettings(XMLelement *node);
	Camera getCamera(XMLelement *node);
	vec3 getAttributeVector(XMLelement *node);
};

