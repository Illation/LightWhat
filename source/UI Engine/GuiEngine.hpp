#pragma once


#include "EngineManagers/WindowManager.hpp"
#include "EngineManagers/EventManager.hpp"

#include "Components\GuiObject.hpp"
#include "Components\Bitmap.hpp"
#include "Components\Font.hpp"
#include "Components\Rectangle.hpp"
class GuiEngine
{
	friend class ProgramController;
public:
	virtual ~GuiEngine();
	static GuiEngine* GetSingleton();//duh

	//Window stuff
	int CreateNewWindow();
	void DestroyWindow(int windowId);
	int GetWidth(int windowId);
	int GetHeight(int windowId);

	//Operating System functions
	std::string GetFileName();
	std::string GetTTFName();
	std::string GetRootDirectory();
	std::string SaveFileName();

	//Setters for Drawing
	void SetWindow(int windowId);
	void SetColor(colRGB color);
	void SetFont(Font *fontPtr);
	void SetDefaultFont();

	//Getters for Drawing
	Font* GetFont();

	//Draw functions
	void DrawBitmap(int x, int y, Bitmap *bmpPtr);
	void DrawString(const std::string &message, int posX, int posY);
	void DrawLine(vec2 pos1, vec2 pos2);
	void DrawRect(Rect rect);
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

	//True if user starts pressing button
	bool IsMouseButtonPressed(int button);
	//True if user is pressing button
	bool IsMouseButtonDown(int button);
	//True if user stops pressing button
	bool IsMouseButtonReleased(int button);
	//Returns mouse position as vector
	vec2 GetMousePosition();

	//GuiObjects
	void RegisterGuiObject(GuiObject *objPtr);
	void UnRegisterGuiObject(GuiObject *objPtr);

protected:
	//functions called by program controller
	//Application functions
	void Init();
	void Clear();
	void PreTick();
	void Paint();
	bool IsExitRequested();
private:
	//Singleton constructor and instance
	GuiEngine();
	static GuiEngine* m_GuiEnginePtr;

	WindowManager *m_WindowPtr = nullptr;
	EventManager *m_EventMngPtr = nullptr;

	std::string m_RootDirectory = string("");
	bool m_Exit = false;

	std::vector<GuiObject*> m_GuiObjPtrArr;

	int m_CurrentWindowId = 0;
	SDL_Color m_Color;
	Font *m_FontPtr = nullptr;
	Font *m_DefaultFontPtr = nullptr;
};

