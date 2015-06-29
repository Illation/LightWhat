#include "LWScene.hpp"


LWScene::LWScene()
{
	Background *bgs = new Background();
	materials.push_back(bgs);
}


LWScene::~LWScene()
{
	clearScene();
}

void LWScene::setupCamera(int camWidth, int camHeight)
{
	cam = Camera(point3(0.3f, -0.3f, 8.f), vec3(0.f, 0.f, -1.f), camWidth, camHeight);
	cam.setTarget(point3(0.f, -0.15f, 0.f));
	cam.setParameters(65.f, 1.f, 50.f);
}

void LWScene::loadFile(string fileName)//in future, pass the lists as references to directly add the shapes to avoid messing up the indices
{
	SceneImporter *import = new SceneImporter();
	import->loadScene(fileName);
	size_t materialIndex = materials.size();
	size_t textureIndex = textures.size();
	for (size_t i = 0; i < import->materials.size(); i++)
	{
		materials.push_back(import->materials[i]);
	}
	for (size_t i = 0; i < import->textures.size(); i++)
	{
		textures.push_back(import->textures[i]);
	}
	for (size_t i = 0; i < import->shapes.size(); i++)
	{
		shapes.push_back(import->shapes[i]);
	}
	delete import;
	import = nullptr;
}

void LWScene::loadTestScene(){
	//Cornell box
	background = colRGB(0, 0, 0);
	DiffuseBRDF *dif1Ptr = new DiffuseBRDF(colRGB(0.9f, 0.1f, 0.1f), 1.0f, false, 0);//Red
	materials.push_back(dif1Ptr);
	DiffuseBRDF *dif2Ptr = new DiffuseBRDF(colRGB(1.0f, 1.0f, 1.0f), 1.0f, false, 0);//White
	materials.push_back(dif2Ptr);
	DiffuseBRDF *dif3Ptr = new DiffuseBRDF(colRGB(0.1f, 0.9f, 0.1f), 1.0f, false, 0);//Green
	materials.push_back(dif3Ptr);
	GlossyBRDF *gloss1Ptr = new GlossyBRDF(colRGB(0.256f, 0.375f, 0.59f), false, 0, 1.f, 13.5f);
	DiffuseBRDF *dif5Ptr = new DiffuseBRDF(colRGB(0.8f, 0.8f, 0.8f), 1.f, false, 0);
	Mix *mix1Ptr = new Mix(dif5Ptr, gloss1Ptr, 0.789f);//Glossy
	materials.push_back(mix1Ptr);
	GlassBRDF *glass1Ptr = new GlassBRDF(colRGB(1.f, 1.f, 1.f), false, 0, 0.1f, 1.3f, (0.3f*(PI/180.f)));
	materials.push_back(glass1Ptr);//Glass
	DiffuseBRDF *dif4Ptr = new DiffuseBRDF(colRGB(0.1f, 0.1f, 0.9f), 1.0f, false, 0);//Blues
	materials.push_back(dif4Ptr);

	Plane *pl0 = new Plane(plane(vec3(1.f, 0.f, 0.f), -2.5f), materialPointer::CUSTOM_BEGIN + 0);
	shapes.push_back(pl0);
	Plane *pl1 = new Plane(plane(vec3(0.f, 1.f, 0.f), -2.5f), materialPointer::CUSTOM_BEGIN + 1);
	shapes.push_back(pl1);
	Plane *pl2 = new Plane(plane(vec3(0.f, 0.f, 1.f), -2.5f), materialPointer::CUSTOM_BEGIN + 1);
	shapes.push_back(pl2);
	Plane *pl3 = new Plane(plane(vec3(0.f, 0.f, -1.f), -9.f), materialPointer::CUSTOM_BEGIN + 1);
	shapes.push_back(pl3);
	Plane *pl4 = new Plane(plane(vec3(0.f, -1.f, 0.f), -2.7f), materialPointer::CUSTOM_BEGIN + 1);
	shapes.push_back(pl4);
	Plane *pl5 = new Plane(plane(vec3(-1.f, 0.f, 0.f), -2.5f), materialPointer::CUSTOM_BEGIN + 2);
	shapes.push_back(pl5);

	Sphere *sp1 = new Sphere(point3(-1.f, -1.5f, -1.f), 1.f, materialPointer::CUSTOM_BEGIN + 3);
	shapes.push_back(sp1);
	Sphere *sp2 = new Sphere(point3(0.3f, -1.7f, 1.f), 0.8f, materialPointer::CUSTOM_BEGIN + 4);
	shapes.push_back(sp2);

	AreaLight *light = new AreaLight(vec3(0.f, 0.f, 1.f), plane(vec3(0.f, -1.f, 0.f), -2.3f), 2.f, 1, colRGB(0.8f, 0.8f, 0.8f), 8.f);
	lights.push_back(light);


	solidifyLights();
}

void LWScene::clearScene(){
	shapes.clear();
	lights.clear();
	materials.clear();
	textures.clear();
}

void LWScene::solidifyLights()
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i]->getType() == LightType::LIGHT_POINT)
		{
			size_t matIdx = materials.size();
			colRGB lightCol;
			Sphere *sp = ((PointLight*)lights[i])->solidify(matIdx, lightCol);
			EmissionBRDF *emPtr = new EmissionBRDF(lightCol, 1.f, false, 0);
			materials.push_back(emPtr);
			shapes.push_back(sp);
		}
		if (lights[i]->getType() == LightType::LIGHT_AREA)
		{
			size_t matIdx = materials.size();
			colRGB lightCol;
			Mesh *lMesh = ((AreaLight*)lights[i])->solidify(matIdx, lightCol);
			EmissionBRDF *emPtr = new EmissionBRDF(lightCol, 1.f, false, 0);
			materials.push_back(emPtr);
			shapes.push_back(lMesh);
		}
	}
}

void LWScene::updateSceneInfo(){
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