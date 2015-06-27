#pragma once
//std includes
#include <string>
#include <vector>
#include <Windows.h>
#include <Commdlg.h>
//sdl includes
#include <SDL/SDL.h>
#include <SDL/SDL_TTF.h>
//light what includes
#include "../Rendering/common.hpp"
#include "Window.hpp"
class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void InitWindow();

	void UpdateWindow();

	int GetWindowWidth();
	int GetWindowHeight();

	SDL_Renderer* GetRenderer();
private:
	Window *m_WindowPtr;
};