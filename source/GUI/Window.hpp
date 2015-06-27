#pragma once
#include <SDL/SDL.h>
class Window
{
public:
	Window(int width, int height, Uint32 flags);
	virtual ~Window();

	void UpdateWindow();

	int GetWidth();
	int GetHeight();

	SDL_Renderer* GetRenderer();
private:
	SDL_Window* m_SdlWindowPtr;
	SDL_Renderer *m_SdlRenderPtr;

	int m_Width;
	int m_Height;
};

