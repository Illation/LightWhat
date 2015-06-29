#include "WindowManager.hpp"


WindowManager::WindowManager()
{
}


WindowManager::~WindowManager()
{
	for (size_t i = 0; i < m_WinPtrArr.size(); i++)
	{
		delete m_WinPtrArr[i];
		m_WinPtrArr[i] = nullptr;
	}
	m_WinPtrArr.clear();
}

int WindowManager::CreateNewWindow()
{
	Window *windowPtr = new Window(1900, 980, 0);
	m_WinPtrArr.push_back(windowPtr);
	return windowPtr->GetID();
}
void WindowManager::DestroyWindow(int windowId)
{
	for (size_t i = 0; i < m_WinPtrArr.size(); i++)
	{
		if (windowId == m_WinPtrArr[i]->GetID())
		{
			delete m_WinPtrArr[i];
			m_WinPtrArr[i] = nullptr;
			m_WinPtrArr.erase(m_WinPtrArr.begin() + i);
			i--;
		}
	}
}
void WindowManager::ClearWindows()
{
	for (size_t i = 0; i < m_WinPtrArr.size(); i++)
	{
		m_WinPtrArr[i]->Clear();
	}
}
void WindowManager::HandleWindowEvent(SDL_Event event)
{
	int windowId =  event.window.windowID;
	for (size_t i = 0; i < m_WinPtrArr.size(); i++)
	{
		if (windowId == m_WinPtrArr[i]->GetID())
		{
			m_WinPtrArr[i]->HandleEvent(event);
		}
		if (m_WinPtrArr[i]->IsClosed())
		{
			delete m_WinPtrArr[i];
			m_WinPtrArr[i] = nullptr;
			m_WinPtrArr.erase(m_WinPtrArr.begin() + i);
			i--;
		}
	}
}
void WindowManager::UpdateWindows()
{
	for (size_t i = 0; i < m_WinPtrArr.size(); i++)
	{
		m_WinPtrArr[i]->Update();
	}
}

Window* WindowManager::GetWindow(int windowId)
{
	for (size_t i = 0; i < m_WinPtrArr.size(); i++)
	{
		if (windowId == m_WinPtrArr[i]->GetID())
		{
			return m_WinPtrArr[i];
		}
	}
	Window* nullWindow = nullptr;
	std::cout << "window id invalid" << std::endl;
	return nullWindow;
}

bool WindowManager::AllWindowsClosed()
{
	return m_WinPtrArr.size() <= 0;
}