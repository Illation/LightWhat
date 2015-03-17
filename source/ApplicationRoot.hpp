#pragma once

#include <Windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_TTF.h>
#include <iostream>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <ctime>

#include "Rendering/Renderer.hpp"
#include "GUI/winmain.hpp"
#include "FileManagment\ImageExporter.hpp"
#include "Rendering\PostProcessor.hpp"

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
	void drawTexture();
	void displaySceneInfo();
	void setPixel(int x, int y, colRGB col);
	void renderText(const std::string &message, TTF_Font *daFont,
		SDL_Color color, int fontSize, int posX, int posY);

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
	int maxSamples = 128;
	bool isSceneLoaded = false;
	size_t dispTexIdx=0;

	string fileName = string("");
	string exeDirectory = string("");

	clock_t start;

	//Layout parameters
	int _screenWidth;
	int _screenHeight;

	int m_ImagePosX;
	int m_ImagePosY;
	int m_ResolutionX;
	int m_ResolutionY;

	int m_TexPosX;
	int m_TexPosY;
	int m_TexResolutionX;
	int m_TexResolutionY;
	Texture *daImage;


	//GUI stuff
	TTF_Font *m_ConsoleFontRegularPtr = nullptr;
	TTF_Font *m_ConsoleFontBoldPtr = nullptr;
	vector<string> sceneInfo;
	string renderTime = string("Render time: - seconds");
	string renderSamples = string("Samples rendered: - ");
};

