#include "SceneLoader.h"

SceneLoader::SceneLoader()
{

}


SceneLoader::~SceneLoader()
{
	delete importer;
	importer = nullptr;
}

bool SceneLoader::loadScene(string FileName){
	importer = new Assimp::Importer();
	const aiScene* m_AssimpScenePtr = importer->ReadFile(FileName, aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	if (!m_AssimpScenePtr)
	{
		cout << "loading scene with assimp failed" << endl;
		return false;
	}
	processModels(m_AssimpScenePtr);
	processMaterials(m_AssimpScenePtr);
	processLights(m_AssimpScenePtr);
	processCamera(m_AssimpScenePtr);
	return true;
}

void SceneLoader::processModels(const aiScene* scene){
	if (scene->HasMeshes())
	{
		for (unsigned int i = 0; i < scene->mNumMeshes; i++){
			Mesh *lMesh = new Mesh();
			//Get Vertices
			if (scene->mMeshes[i]->HasPositions())
			{
				aiMesh *daMesh = scene->mMeshes[i];
				unsigned int vertNum = daMesh->mNumVertices;
				for (unsigned int j = 0; j < vertNum; j++){
					point3 lVert;
					lVert.x = daMesh->mVertices[j].x;
					lVert.y = daMesh->mVertices[j].z;
					lVert.z = -(daMesh->mVertices[j].y);
					lMesh->addVertex(lVert);
				}
				//Get Normals
				if (daMesh->HasNormals()){
					for (unsigned int j = 0; j < daMesh->mNumVertices; j++){
						vec3 lNorm;
						lNorm.x = daMesh->mNormals[j].x;
						lNorm.y = daMesh->mNormals[j].z;
						lNorm.z = -(daMesh->mNormals[j].y);
						lMesh->addNormal(lNorm);
					}
					//Get UVs
					bool hasUV = false;
					for (unsigned int j = 0; j < daMesh->GetNumUVChannels(); j++){
						if (daMesh->HasTextureCoords(j)){
							hasUV = true;
							lMesh->addUVset(string("UV_set_") + to_string(j), true);
							for (unsigned int k = 0; k < vertNum; k++){
								point2 lUV;
								lUV.x = daMesh->mTextureCoords[j][k].x;
								lUV.y = daMesh->mTextureCoords[j][k].y;
								lMesh->addUV(lUV, j);
							}
						}
					}
					//Create Polylist
					unsigned int matIndex = CUSTOM_BEGIN + scene->mMeshes[i]->mMaterialIndex;
					lMesh->addPolyList(matIndex, hasUV);
					if (scene->mMeshes[i]->HasFaces()){
						for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++){
							aiFace* face = &scene->mMeshes[i]->mFaces[j];
							if (face->mNumIndices == 3){
								tri thisTri;
								thisTri.vertA = face->mIndices[0];
								thisTri.vertB = face->mIndices[1];
								thisTri.vertC = face->mIndices[2];
								thisTri.normA = face->mIndices[0];
								thisTri.normB = face->mIndices[1];
								thisTri.normC = face->mIndices[2];
								thisTri.uvA = face->mIndices[0];
								thisTri.uvB = face->mIndices[1];
								thisTri.uvC = face->mIndices[2];
								lMesh->createTri(thisTri, 0);
							}

						}
						shapes.push_back(lMesh);
					}
				}
				else //no normals in file, generate own normals
				{

				}
			}
		}
	}
}

void SceneLoader::processMaterials(const aiScene* scene){
	materials.push_back(Shader(BACKGROUND, colRGB(0, 10, 20) / 255, colRGB(0, 0, 0), PhongParameters()));
	if (scene->HasMaterials())
	{
		aiMaterial** aiMaterials = scene->mMaterials;
		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			Shader lShade;
			lShade.shade = DIFFUSE;
			aiShadingMode aiModel = aiShadingMode_Gouraud;
			aiMaterials[i]->Get(AI_MATKEY_SHADING_MODEL, aiModel);
			if (aiModel = aiShadingMode_Phong)
			{
				lShade.shade = PHONG;
			}
			if (aiModel = aiShadingMode_Blinn)
			{
				lShade.shade = REFLECT;
			}
			lShade.diffuse = colRGB(0.5, 0.5, 0.5);
			aiColor3D aiDif;
			if (AI_SUCCESS != aiMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, aiDif)) {
				// handle epic failure here
			}
			else{
				lShade.diffuse.red = aiDif.r;
				lShade.diffuse.green = aiDif.g;
				lShade.diffuse.blue = aiDif.b;
				if (!lShade.shade == DIFFUSE)
				{
					lShade.specular = colRGB(0.5, 0.5, 0.5);
					aiColor3D aiSpec;
					if (AI_SUCCESS != aiMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, aiSpec)) {
						// handle epic failure here
					}
					else{
						lShade.specular.red = aiSpec.r;
						lShade.specular.green = aiSpec.g;
						lShade.specular.blue = aiSpec.b;
						lShade.param = PhongParameters(1, 1, 1, 50);
						float aiPhongExponent;
						if (AI_SUCCESS != aiMaterials[i]->Get(AI_MATKEY_SHININESS, aiPhongExponent)) {
							// handle epic failure here
						}
						else lShade.param.ke = (double)aiPhongExponent;
						float aiSpecularScale;
						if (AI_SUCCESS != aiMaterials[i]->Get(AI_MATKEY_SHININESS_STRENGTH, aiSpecularScale)) {
							// handle epic failure here
						}
						else lShade.param.ks = (double)aiSpecularScale;
					}
				}
			}
			materials.push_back(lShade);
		}
	}
}

bool SceneLoader::processLights(const aiScene* scene){
	if (!scene->HasLights())
	{
		return false;
	}
	else
	{
		aiLight** mLights = scene->mLights;
		for (unsigned int i = 0; i < scene->mNumLights; i++)
		{
			Light thisLight;
			thisLight.center.x = mLights[i]->mPosition.x;
			thisLight.center.y = mLights[i]->mPosition.y;
			thisLight.center.z = -(mLights[i]->mPosition.z);
			thisLight.col.red = mLights[i]->mColorDiffuse.r;
			thisLight.col.green = mLights[i]->mColorDiffuse.g;
			thisLight.col.blue = mLights[i]->mColorDiffuse.b;
			thisLight.intensity = (double)mLights[i]->mAttenuationConstant;
			lights.push_back(thisLight);
		}
		return true;
	}
}

bool SceneLoader::processCamera(const aiScene* scene){
	return false; //placeholder
}