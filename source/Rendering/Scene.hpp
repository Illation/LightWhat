#pragma once

#include "../FileManagment/SceneLoader.hpp"
#include "shape.hpp"
#include "shapes/Sphere.hpp"
#include "shapes/Plane.hpp"
#include "light.hpp"
#include "lights/PointLight.hpp"
#include "lights/AreaLight.hpp"
#include "../Rendering/Texture.hpp"

enum materialPointer{
	BACKGROUND_MATERIAL,
	CUSTOM_BEGIN
};

class Scene
{
public:
	Scene();
	virtual ~Scene();

	void setupCamera(int camWidth, int camHeight);

	void loadFile(string fileName);
	void loadTestScene();
	void solidifyLights();
	void clearScene();

	Camera cam;
	vector  <shape*>shapes;
	vector  <light*>lights;
	vector  <Shader>materials;
	vector  <Texture>textures;
	colRGB background = colRGB(132, 149, 139) / 255;

	void updateSceneInfo();
	int vertCount, triCount, objCount, lightCount;
};

