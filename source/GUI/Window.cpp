#include "Window.hpp"


Window::Window(int width, int height, Uint32 flags)
{
	m_SdlWindowPtr = SDL_CreateWindow("LightWhat - Robert Lindner", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

	SDL_GetWindowSize(m_SdlWindowPtr, &m_Width, &m_Height);
	m_SdlRenderPtr = SDL_CreateRenderer(m_SdlWindowPtr, -1, 0);
}

Window::~Window()
{
	SDL_DestroyRenderer(m_SdlRenderPtr);
	SDL_DestroyWindow(m_SdlWindowPtr);
}

void Window::UpdateWindow()
{
	SDL_RenderPresent(m_SdlRenderPtr);
}

int Window::GetWidth()
{
	return m_Width;
}

int Window::GetHeight()
{
	return m_Height;
}

SDL_Renderer* Window::GetRenderer()
{
	return m_SdlRenderPtr;
}