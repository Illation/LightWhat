#include "Window.hpp"


Window::Window(int width, int height, Uint32 flags)
{
	m_SdlWindowPtr = SDL_CreateWindow("LightWhat - Robert Lindner", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

	SDL_GetWindowSize(m_SdlWindowPtr, &m_Width, &m_Height);
	m_SdlRenderPtr = SDL_CreateRenderer(m_SdlWindowPtr, -1, 0);

	m_ID = SDL_GetWindowID(m_SdlWindowPtr);
}

Window::~Window()
{
	SDL_DestroyRenderer(m_SdlRenderPtr);
	SDL_DestroyWindow(m_SdlWindowPtr);
}

void Window::Clear()
{
	SDL_RenderClear(m_SdlRenderPtr);
}

void Window::HandleEvent(SDL_Event event)
{
	switch (event.window.event)
	{
	case SDL_WINDOWEVENT_CLOSE:
		m_IsClosed = true;
		break;
	}
}

void Window::Update()
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

int Window::GetID()
{
	return m_ID;
}

bool Window::IsClosed()
{
	return m_IsClosed;
}

SDL_Renderer* Window::GetRenderer()
{
	return m_SdlRenderPtr;
}