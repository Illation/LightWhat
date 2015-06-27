#pragma once
#include "WindowManager.hpp"
#include "EventManager.hpp"

#include "Components\Bitmap.hpp"
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

	//General getters
	WindowManager* GetWindow();
	int GetWidth();
	int GetHeight();
	bool IsExitRequested();
	//Windows functions
	std::string GetFileName();
	std::string GetTTFName();
	std::string SaveFileName();

	//Draw functions
	void DrawBitmap(int x, int y, Bitmap *bmpPtr);
	void DrawString(const std::string &message, TTF_Font *daFont,
		SDL_Color color, int fontSize, int posX, int posY);
	void FillRect(Rect rect);

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
};

