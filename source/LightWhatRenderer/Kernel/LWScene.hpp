#pragma once

#include "../FileManagment/SceneImporter.hpp"
#include "../Utility/Texture.hpp"
#include "../Geometry/shape.hpp"
#include "../Geometry/shapes/Sphere.hpp"
#include "../Geometry/shapes/Plane.hpp"
#include "../Geometry/light.hpp"
#include "../Geometry/lights/PointLight.hpp"
#include "../Geometry/lights/AreaLight.hpp"
#include "../Shading/shader.hpp"
#include "../Shading/shaders/Background.hpp"
#include "../Shading/shaders/DiffuseBRDF.hpp"
#include "../Shading/shaders/EmissionBRDF.hpp"
#include "../Shading/shaders/GlassBRDF.hpp"
#include "../Shading/shaders/GlossyBRDF.hpp"
#include "../Shading/shaders/Mix.hpp"

enum materialPointer{
	BACKGROUND_MATERIAL,
	CUSTOM_BEGIN
};

class LWScene
{
public:
	LWScene();
	virtual ~LWScene();

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

