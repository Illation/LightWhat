#include "WindowManager.hpp"


WindowManager::WindowManager()
{
}


WindowManager::~WindowManager()
{
	delete m_WindowPtr;
	m_WindowPtr = nullptr;
}

void WindowManager::InitWindow()
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	TTF_Init();
	atexit(TTF_Quit);

	m_WindowPtr = new Window(1900, 980, 0);
}
void WindowManager::UpdateWindow()
{
	m_WindowPtr->UpdateWindow();
}

int WindowManager::GetWindowWidth()
{
	return m_WindowPtr->GetWidth();
}

int WindowManager::GetWindowHeight()
{
	return m_WindowPtr->GetHeight();
}

SDL_Renderer* WindowManager::GetRenderer()
{
	return m_WindowPtr->GetRenderer();
}