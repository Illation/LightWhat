#pragma once
#include <SDL/SDL.h>
class Window
{
public:
	Window(int width, int height, Uint32 flags);
	virtual ~Window();

	void Clear();
	void HandleEvent(SDL_Event event);
	void Update();

	int GetWidth();
	int GetHeight();
	int GetID();
	bool IsClosed();

	SDL_Renderer* GetRenderer();
private:
	SDL_Window* m_SdlWindowPtr;
	SDL_Renderer *m_SdlRenderPtr;

	int m_Width;
	int m_Height;
	int m_ID;

	bool m_IsClosed = false;
};

