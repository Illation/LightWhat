//Assimp copyright notice:
//Copyright(c) 2006 - 2012 assimp team
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met :
//
//Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and / or other materials provided with the distribution.
//Neither the name of the assimp team nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
//PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, 
//PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
//OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE

#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>  
#include <assimp/postprocess.h>
#include "../Rendering/shapes/Mesh.hpp"
#include "../Rendering/Scene.hpp"
#include "../Rendering/Texture.hpp"
#include "../Rendering/light.hpp"
#include "../Rendering/lights/PointLight.hpp"
#include "TextureLoader.hpp"
#include "../Rendering/shaders/DiffuseBRDF.hpp"
#include "../Rendering/shaders/GlossyBRDF.hpp"
#include "../Rendering/shaders/Mix.hpp"
using namespace std;
class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader();

	bool loadScene(string FileName);//


	Camera cam;
	vector  <shape*>shapes;
	vector  <light*>lights;
	vector  <shader*>materials;
	vector  <Texture>textures;
private:
	string directoryName;
	void processModels(const aiScene* scene);
	bool processLights(const aiScene* scene);
	bool processCamera(const aiScene* scene);
	void processMaterials(const aiScene* scene);//
	Assimp::Importer *importer = nullptr;
};