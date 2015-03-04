#pragma once

#include "../FileManagment/SceneLoader.h"
#include "shape.h"
#include "shapes/Sphere.h"
#include "shapes/Plane.h"
#include "../Rendering/Texture.h"

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
	void clearScene();

	Camera cam;
	vector  <shape*>shapes;
	vector  <Light>lights;
	vector  <Shader>materials;
	vector  <Texture>textures;
	colRGB background = colRGB(132, 149, 139) / 255;

	void updateSceneInfo();
	int vertCount, triCount, objCount, lightCount;
};

