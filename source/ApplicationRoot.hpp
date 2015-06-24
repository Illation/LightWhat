#pragma once

#include <Windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_TTF.h>
#include <iostream>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <ctime>

#include "Rendering/LWRenderer.hpp"
#include "GUI/WindowManager.hpp"
#include "GUI/Components/Bitmap.hpp"
#include "FileManagment\ImageExporter.hpp"
#include "FileManagment\SceneDescriptionLoader.hpp"
#include "Rendering\PostProcessor.hpp"

class ApplicationRoot
{
public:
	ApplicationRoot();
	~ApplicationRoot();

	void run();
	void end();

private:

	void initSystems();
	void functionLoop();
	void processInput();
	void updateImage();
	void drawTexture();

	bool exit = false;

	LWRenderer *m_Renderer = nullptr;
	ImageExporter *imgExp = nullptr;

	size_t dispTexIdx=0;

	string fileName = string("");
	string exeDirectory = string("");

	//Layout parameters

	int m_ImagePosX;
	int m_ImagePosY;
	int m_ResolutionX;
	int m_ResolutionY;
	Bitmap* m_BmpResultPtr;

	int m_TexPosX;
	int m_TexPosY;
	int m_TexResolutionX;
	int m_TexResolutionY;
	Bitmap* m_BmpTexturePtr;
	bool m_DrawTexture = false;


	//GUI stuff
	TTF_Font *m_ConsoleFontRegularPtr = nullptr;
	TTF_Font *m_ConsoleFontBoldPtr = nullptr;
	vector<string> sceneInfo;
};

