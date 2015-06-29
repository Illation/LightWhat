#pragma once
#include "XMLParser.hpp"
#include "XMLdom.hpp"
#include "../../../LightWhatRenderer/Kernel/LWScene.hpp"
#include "../../../LightWhatRenderer/Kernel/LWSettings.hpp"

class SceneDescriptionLoader
{
public:
	SceneDescriptionLoader();
	~SceneDescriptionLoader();

	void loadSceneDescriptionFile(const char *filename, LWSettings *settings);

	Camera cam;
	vector  <shape*>shapes;
	vector  <light*>lights;
	vector  <shader*>materials;
	vector  <Texture>textures;

private:
	void getSettings(XMLelement *node, LWSettings *settings);
	Camera getCamera(XMLelement *node, LWSettings *settings);
	vec3 getAttributeVector(XMLelement *node);
};

