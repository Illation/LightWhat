#pragma once
//std includes
#include <vector>
#include <iostream>
//sdl includes
#include <SDL/SDL.h>
//light what includes
#include "../../LightWhatRenderer/Utility/common.hpp"
#include "../Components/Window.hpp"
class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	// Creates a Window and returns the ID
	int CreateNewWindow();
	// Destroys the Window. Window ID becomes invalid!
	void DestroyWindow(int windowId);

	//Eventloop functions
	void ClearWindows();
	void HandleWindowEvent(SDL_Event);
	void UpdateWindows();

	//Getters
	Window* GetWindow(int windowId);
	Window* operator[](int windowId);
	bool HasWindow(int windowId);
	bool AllWindowsClosed();
private:
	std::vector<Window*> m_WinPtrArr;
	bool m_WindowEverCreated = false;
};