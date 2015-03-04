#include "Scene.h"


Scene::Scene()
{
	Shader bgs = Shader(BACKGROUND, colRGB(0, 10, 20) / 255, colRGB(0, 0, 0), PhongParameters());
	bgs.hasDifTex = false;
	materials.push_back(bgs);
}


Scene::~Scene()
{
	clearScene();
}

void Scene::setupCamera(int camWidth, int camHeight)
{
	cam = Camera(point3(1380.3, 422.3, 999), vec3(0, 0, -1), camWidth, camHeight);
	cam.setTarget(point3(037, 180.8, 016.5));
	cam.setParameters(59.559, 1, 50);
}

void Scene::loadFile(string fileName)
{
	SceneLoader *import = new SceneLoader();
	import->loadScene(fileName);
	shapes = import->shapes;
	materials = import->materials;
	textures = import->textures;
		//setup Lights
		Light light;
		light.center = point3(363.9, 1247.4, 326.4);
		light.col = colRGB(1, 1, 1);
		light.intensity = 3;
		lights.push_back(light);
	delete import;
	import = nullptr;
}

void Scene::loadTestScene(){
	background = colRGB(0, 0, 0);
	materials.push_back(Shader(REFLECT, colRGB(0.7, 0.7, 0.7), colRGB(1, 1, 1), PhongParameters(1, 1, 0.6, 50)));
	materials.push_back(Shader(REFLECT, colRGB(0.7, 0.7, 1), colRGB(1, 1, 1), PhongParameters(1, 0.1, 1, 50)));
	materials.push_back(Shader(REFLECT, colRGB(0.4, 0.3, 0.3), colRGB(0, 0, 0), PhongParameters(1, 1, 0, 50)));
	Sphere *sp1 = new Sphere(point3(1, -0.8, -3), 2.5, materialPointer::CUSTOM_BEGIN);
	shapes.push_back(sp1);
	Sphere *sp2 = new Sphere(point3(-5.5, -0.5, -7), 2, materialPointer::CUSTOM_BEGIN + 1);
	shapes.push_back(sp2);
	Plane *pl1 = new Plane(plane(vec3(0, 1, 0), -4.4), materialPointer::CUSTOM_BEGIN + 2);
	shapes.push_back(pl1);
	Light light;
	light.center = point3(0, 5, -5);
	light.col = colRGB(0.6, 0.6, 0.6);
	light.intensity = 1;
	lights.push_back(light);
	Light light2;
	light2.center = point3(2, 5, -1);
	light2.col = colRGB(0.7, 0.7, 0.9);
	light2.intensity = 1;
	lights.push_back(light2);
}

void Scene::clearScene(){
	shapes.clear();
	lights.clear();
	materials.clear();
	textures.clear();
}

void Scene::updateSceneInfo(){
	vertCount = 0;
	triCount = 0;
	objCount = shapes.size();
	lightCount = lights.size();
	for (int i = 0; i < objCount; i++)
	{
		if (shapes[i]->getType() == MESH)
		{
			vertCount += ((Mesh*)shapes[i])->getVertCount();
			triCount += ((Mesh*)shapes[i])->getTriCount();
		}
	}
}