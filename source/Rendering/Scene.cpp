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
	cam = Camera(point3(0.3f, -0.3f, 8.f), vec3(0.f, 0.f, -1.f), camWidth, camHeight);//(point3(5.3f, 16.f, -8.f), vec3(0.f, 0.f, -1.f), camWidth, camHeight); //
	cam.setTarget(point3(0.f, -0.15f, 0.f));
	cam.setParameters(65.f, 1.f, 50.f);
}

void Scene::loadFile(string fileName)
{
	SceneLoader *import = new SceneLoader();
	import->loadScene(fileName);
	shapes = import->shapes;
	materials = import->materials;
	textures = import->textures;
		//setup Lights
	PointLight *light = new PointLight(point3(1, 8, 4), colRGB(1, 1, 1), 3.f);
	lights.push_back(light);
	delete import;
	import = nullptr;

	solidifyLights();
}

void Scene::loadTestScene(){
	//Cornell box
	background = colRGB(0, 0, 0);
	materials.push_back(Shader(DIFFUSE, colRGB(0.9f, 0.1f, 0.1f), colRGB(1.f, 1.f, 1.f), PhongParameters(1.f, 0.1f, 1.f, 50.f)));
	materials.push_back(Shader(DIFFUSE, colRGB(1.0f, 1.0f, 1.0f), colRGB(1.f, 1.f, 1.f), PhongParameters(1.f, 1.f, 0.6f, 50.f)));
	materials.push_back(Shader(DIFFUSE, colRGB(0.1f, 0.9f, 0.1f), colRGB(1.f, 1.f, 1.f), PhongParameters(1.f, 1.f, 0.6f, 50.f)));

	materials.push_back(Shader(REFLECT, colRGB(0.8f, 0.8f, 0.8f), colRGB(1.f, 1.f, 1.f), PhongParameters(1.f, 1.f, 0.7f, 50.f)));
	PhongParameters par = PhongParameters(1.f, 0.05f, 0.5f, 50.f);
	par.refl = 0.18f;
	par.refr = 0.9f;
	par.ior = 1.33f;
	materials.push_back(Shader(GLASS, colRGB(0.6f, 0.6f, 1.f), colRGB(1.f, 1.f, 1.f), par));

	Plane *pl0 = new Plane(plane(vec3(1.f, 0.f, 0.f), -2.5f), materialPointer::CUSTOM_BEGIN + 0);
	shapes.push_back(pl0);
	Plane *pl1 = new Plane(plane(vec3(0.f, 1.f, 0.f), -2.5f), materialPointer::CUSTOM_BEGIN + 1);
	shapes.push_back(pl1);
	Plane *pl2 = new Plane(plane(vec3(0.f, 0.f, 1.f), -2.5f), materialPointer::CUSTOM_BEGIN + 1);
	shapes.push_back(pl2);
	Plane *pl3 = new Plane(plane(vec3(0.f, 0.f, -1.f), -5.f), materialPointer::CUSTOM_BEGIN + 1);
	shapes.push_back(pl3);
	Plane *pl4 = new Plane(plane(vec3(0.f, -1.f, 0.f), -2.7f), materialPointer::CUSTOM_BEGIN + 1);
	shapes.push_back(pl4);
	Plane *pl5 = new Plane(plane(vec3(-1.f, 0.f, 0.f), -2.5f), materialPointer::CUSTOM_BEGIN + 2);
	shapes.push_back(pl5);

	Sphere *sp1 = new Sphere(point3(-1.f, -1.5f, -1.f), 1.f, materialPointer::CUSTOM_BEGIN + 3);
	shapes.push_back(sp1);
	Sphere *sp2 = new Sphere(point3(0.3f, -1.7f, 1.f), 0.8f, materialPointer::CUSTOM_BEGIN + 4);
	shapes.push_back(sp2);

	AreaLight *light = new AreaLight(vec3(0.f, 0.f, 1.f), plane(vec3(0.f, -1.f, 0.f), -2.3f), 2.f, 1, colRGB(1.f, 1.f, 1.f), 1.f);
	lights.push_back(light);


	solidifyLights();
}

void Scene::clearScene(){
	shapes.clear();
	lights.clear();
	materials.clear();
	textures.clear();
}

void Scene::solidifyLights()
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i]->getType() == LightType::LIGHT_POINT)
		{
			size_t matIdx = materials.size();
			colRGB lightCol;
			Sphere *sp = ((PointLight*)lights[i])->solidify(matIdx, lightCol);
			materials.push_back(Shader(FLAT, lightCol, colRGB(0.f, 0.f, 0.f), PhongParameters(1.f, 1.f, 0.f, 0.f)));
			shapes.push_back(sp);
		}
		if (lights[i]->getType() == LightType::LIGHT_AREA)
		{
			size_t matIdx = materials.size();
			colRGB lightCol;
			Mesh *lMesh = ((AreaLight*)lights[i])->solidify(matIdx, lightCol);
			materials.push_back(Shader(FLAT, lightCol, colRGB(0.f, 0.f, 0.f), PhongParameters(1.f, 1.f, 0.f, 0.f)));
			shapes.push_back(lMesh);
		}
	}
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