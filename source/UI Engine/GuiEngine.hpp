#pragma once
#include <Windows.h>
#include <Commdlg.h>

#include <SDL/SDL_TTF.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "EngineManagers/WindowManager.hpp"
#include "EngineManagers/EventManager.hpp"

#include "Components\Bitmap.hpp"
#include "Components\Font.hpp"
#include "Components\Rectangle.hpp"
class GuiEngine
{
public:
	virtual ~GuiEngine();
	static GuiEngine* GetSingleton();//duh

	//Application functions
	void Init();
	void Clear();
	void PreTick();
	void Paint();

	//Window stuff
	int CreateNewWindow();
	void DestroyWindow(int windowId);
	//General getters
	WindowManager* GetWindow();
	int GetWidth(int windowId);
	int GetHeight(int windowId);
	bool IsExitRequested();
	//Windows functions
	std::string GetFileName();
	std::string GetTTFName();
	std::string GetRootDirectory();
	std::string SaveFileName();

	//Draw functions
	void DrawBitmap(int windowId, int x, int y, Bitmap *bmpPtr);
	void DrawString(int windowId, const std::string &message, Font *fontPtr, int posX, int posY);
	void FillRect(int windowId, Rect rect);

	//Input

	// True if user starts pressing key
	bool IsKeyboardKeyPressed(SDL_Scancode key);
	// True if user starts pressing key
	// Supported chars are CAPITAL letters and numbers
	bool IsKeyboardKeyPressed(char key);
	// True if user is pressing key
	bool IsKeyboardKeyDown(SDL_Scancode key);
	// True if user is pressing key
	// Supported chars are CAPITAL letters and numbers
	bool IsKeyboardKeyDown(char key);
	// True if user stops pressing key
	bool IsKeyboardKeyReleased(SDL_Scancode key);
	// True if user stops pressing key
	// Supported chars are CAPITAL letters and numbers
	bool IsKeyboardKeyReleased(char key);
private:
	//Singleton constructor and instance
	GuiEngine();
	static GuiEngine* m_GuiEnginePtr;

	WindowManager *m_WindowPtr = nullptr;
	EventManager *m_EventMngPtr = nullptr;
	HWND m_WindowsWindow;

	std::string m_RootDirectory = string("");
	bool m_Exit = false;
};

