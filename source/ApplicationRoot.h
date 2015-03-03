#pragma once

#include <Windows.h>
#include <SDL/SDL.h>
#include <iostream>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <ctime>

#include "Rendering/Renderer.h"
#include "GUI/winmain.h"
#include "FileManagment\ImageExporter.h"
#include "Rendering\PostProcessor.h"

enum class RenderingState{SETUP, RENDER, EXIT};
enum class PerformanceMode{HIGH, VIEW};

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
	void drawImage();
	void setPixel(int x, int y, colRGB col);

	SDL_Window* _window;
	SDL_Renderer *sdlRenderer;
	SDL_Texture *renderTex; 
	Uint32 * pixels;

	Scene *scPtr = nullptr;
	Renderer *renderer=nullptr;
	PostProcessor postPr;
	ImageExporter *imgExp = nullptr;
	winmain *guiFunctions = nullptr;
	HWND ofWnd;

	RenderingState _state;
	PerformanceMode _mode;
	int m_ColsRendered = 0;
	bool isSceneLoaded = false;

	string fileName = string("");

	clock_t start;

	//Layout parameters
	int m_ImagePosX;
	int m_ImagePosY;
	int _screenWidth;
	int _screenHeight;

	int m_ResolutionX;
	int m_ResolutionY;
	vector<vector<colRGB> > daImage;

};

