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
#include "GUI/winmain.hpp"
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

	bool exit = false;

	LWRenderer *m_Renderer = nullptr;
	ImageExporter *imgExp = nullptr;
	winmain *guiFunctions = nullptr;
	HWND ofWnd;

	size_t dispTexIdx=0;

	string fileName = string("");
	string exeDirectory = string("");

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


	//GUI stuff
	TTF_Font *m_ConsoleFontRegularPtr = nullptr;
	TTF_Font *m_ConsoleFontBoldPtr = nullptr;
	vector<string> sceneInfo;
};

