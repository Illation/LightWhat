#pragma once

#include "../FileManagment/SceneLoader.hpp"
#include "Texture.hpp"
#include "shape.hpp"
#include "shapes/Sphere.hpp"
#include "shapes/Plane.hpp"
#include "light.hpp"
#include "lights/PointLight.hpp"
#include "lights/AreaLight.hpp"
#include "shader.hpp"
#include "shaders/Background.hpp"
#include "shaders/DiffuseBRDF.hpp"
#include "shaders/EmissionBRDF.hpp"
#include "shaders/GlassBRDF.hpp"
#include "shaders/GlossyBRDF.hpp"
#include "shaders/Mix.hpp"

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
	vector  <shader*>materials;
	vector  <Texture>textures;
	colRGB background = colRGB(132, 149, 139) / 255;

	void updateSceneInfo();
	int vertCount, triCount, objCount, lightCount;
};

