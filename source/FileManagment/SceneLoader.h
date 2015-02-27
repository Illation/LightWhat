//Using Assimp:
//Copyright (c) 2006-2012 assimp team
//All rights reserved

#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>  
#include <assimp/postprocess.h>
#include "../Rendering/shapes/Mesh.h"
#include "../Rendering/Scene.h"
using namespace std;
class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader();

	bool loadScene(string FileName);//


	Camera cam;
	vector  <shape*>shapes;
	vector  <Light>lights;
	vector  <Shader>materials;
private:
	void processModels(const aiScene* scene);
	bool processLights(const aiScene* scene);
	bool processCamera(const aiScene* scene);
	void processMaterials(const aiScene* scene);//
	Assimp::Importer *importer = nullptr;
};