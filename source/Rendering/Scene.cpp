#include "Scene.hpp"


Scene::Scene()
{
	Shader bgs = Shader(BACKGROUND, colRGB(132, 149, 139) / 255, colRGB(0, 0, 0), PhongParameters());
	bgs.hasDifTex = false;
	materials.push_back(bgs);
}


Scene::~Scene()
{
	clearScene();
}

void Scene::setupCamera(int camWidth, int camHeight)
{
	cam = Camera(point3(0.f, 0.3f, 5.f), vec3(0.f, 0.f, -1.f), camWidth, camHeight);
	cam.setTarget(point3(0.f, 0.15f, 0.f));
	cam.setParameters(55.f, 1.f, 50.f);
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
		light.center = point3(1, 8, 4);
		light.col = colRGB(1, 1, 1);
		light.intensity = 3;
		lights.push_back(light);
	delete import;
	import = nullptr;
}

void Scene::loadTestScene(){
	background = colRGB(0, 0, 0);
	materials.push_back(Shader(REFLECT, colRGB(0.7f, 0.7f, 0.7f), colRGB(1.f, 1.f, 1.f), PhongParameters(1.f, 1.f, 0.6f, 50.f)));
	materials.push_back(Shader(REFLECT, colRGB(0.7f, 0.7f, 1.f), colRGB(1.f, 1.f, 1.f), PhongParameters(1.f, 0.1f, 1.f, 50.f)));
	materials.push_back(Shader(REFLECT, colRGB(0.4f, 0.3f, 0.3f), colRGB(0.f, 0.f, 0.f), PhongParameters(1.f, 1.f, 0.f, 50.f)));
	Sphere *sp1 = new Sphere(point3(1.f, -0.8f, -3.f), 2.5f, materialPointer::CUSTOM_BEGIN);
	shapes.push_back(sp1);
	Sphere *sp2 = new Sphere(point3(-5.5f, -0.5f, -7.f), 2.f, materialPointer::CUSTOM_BEGIN + 1);
	shapes.push_back(sp2);
	Plane *pl1 = new Plane(plane(vec3(0.f, 1.f, 0.f), -4.4f), materialPointer::CUSTOM_BEGIN + 2);
	shapes.push_back(pl1);
	Light light;
	light.center = point3(0.f, 5.f, -5.f);
	light.col = colRGB(0.6f, 0.6f, 0.6f);
	light.intensity = 1;
	lights.push_back(light);
	Light light2;
	light2.center = point3(2.f, 5.f, -1.f);
	light2.col = colRGB(0.7f, 0.7f, 0.9f);
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