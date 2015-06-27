#pragma once
#include "WindowManager.hpp"
#include <map>
//----------------------------
//Event Manager class definintion
//----------------------------
class EventManager
{
public:
	//Constructor Destructor
	EventManager(WindowManager *winManagerPtr);
	virtual ~EventManager();
	//----------------------------
	//Member functions
	//----------------------------

	//General

	void Init();//call after SDL init
	void UpdateEvents();//call before all GUI ticks
	//Getters

	bool IsExitRequested();
	//Keyboard

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
	//----------------------------
	// Member functions
	//----------------------------

	// Gets the SDL_Scancode for a char
	// Returns false if char is not mapped
	bool GetScancode(SDL_Scancode &code, char key);

	//----------------------------
	// Data Members
	//----------------------------

	//Window Input
	WindowManager *m_WinManagerPtr;

	//Key Input
	Uint8 *m_KeyMapNew = nullptr,
		*m_KeyMapOld = nullptr;
	const Uint8 *m_KeyMapSdl = nullptr;
	int m_KeyboardLength;
	std::map<char, SDL_Scancode> m_CharToSdlMap;

	//Application flow
	bool m_ExitRequested = false;
};

