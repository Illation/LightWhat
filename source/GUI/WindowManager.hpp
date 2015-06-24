#pragma once

#include <string>

#include <Windows.h>
#include <Commdlg.h>
#include <SDL/SDL.h>
#include <SDL/SDL_TTF.h>

#include <vector>
#include "../Rendering/common.hpp"

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void InitWindow();

	std::string GetFileName();
	std::string GetTTFName();
	std::string SaveFileName();

	void RenderText(const std::string &message, TTF_Font *daFont,
		SDL_Color color, int fontSize, int posX, int posY);
	//void SetPixel(int x, int y, colRGB col);
	void UpdateWindow();// std::vector<SDL_Texture*> sdlTexturePtrArr);

	int GetWindowWidth();
	int GetWindowHeight();

	SDL_Renderer* GetRenderer();
private:
	SDL_Window* _window;
	SDL_Renderer *sdlRenderer;
	//SDL_Texture *renderTex;
	//Uint32 * pixels;
	HWND ofWnd;

	int _screenWidth;
	int _screenHeight;
};