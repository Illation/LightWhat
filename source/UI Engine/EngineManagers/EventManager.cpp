#include "EventManager.hpp"

//----------------------------
//Constructor and Destructor
//----------------------------
EventManager::EventManager(WindowManager *winManagerPtr)
	:m_WinManagerPtr(winManagerPtr)
{
	m_CharToSdlMap['A'] = SDL_SCANCODE_A;
	m_CharToSdlMap['B'] = SDL_SCANCODE_B;
	m_CharToSdlMap['C'] = SDL_SCANCODE_C;
	m_CharToSdlMap['D'] = SDL_SCANCODE_D;
	m_CharToSdlMap['E'] = SDL_SCANCODE_E;
	m_CharToSdlMap['F'] = SDL_SCANCODE_F;
	m_CharToSdlMap['G'] = SDL_SCANCODE_G;
	m_CharToSdlMap['H'] = SDL_SCANCODE_H;
	m_CharToSdlMap['I'] = SDL_SCANCODE_I;
	m_CharToSdlMap['J'] = SDL_SCANCODE_J;
	m_CharToSdlMap['K'] = SDL_SCANCODE_K;
	m_CharToSdlMap['L'] = SDL_SCANCODE_L;
	m_CharToSdlMap['M'] = SDL_SCANCODE_M;
	m_CharToSdlMap['N'] = SDL_SCANCODE_N;
	m_CharToSdlMap['O'] = SDL_SCANCODE_O;
	m_CharToSdlMap['P'] = SDL_SCANCODE_P;
	m_CharToSdlMap['Q'] = SDL_SCANCODE_Q;
	m_CharToSdlMap['R'] = SDL_SCANCODE_R;
	m_CharToSdlMap['S'] = SDL_SCANCODE_S;
	m_CharToSdlMap['T'] = SDL_SCANCODE_T;
	m_CharToSdlMap['U'] = SDL_SCANCODE_U;
	m_CharToSdlMap['V'] = SDL_SCANCODE_V;
	m_CharToSdlMap['W'] = SDL_SCANCODE_W;
	m_CharToSdlMap['X'] = SDL_SCANCODE_X;
	m_CharToSdlMap['Y'] = SDL_SCANCODE_Y;
	m_CharToSdlMap['Z'] = SDL_SCANCODE_Z;

	m_CharToSdlMap['0'] = SDL_SCANCODE_0;
	m_CharToSdlMap['1'] = SDL_SCANCODE_1;
	m_CharToSdlMap['2'] = SDL_SCANCODE_2;
	m_CharToSdlMap['3'] = SDL_SCANCODE_3;
	m_CharToSdlMap['4'] = SDL_SCANCODE_4;
	m_CharToSdlMap['5'] = SDL_SCANCODE_5;
	m_CharToSdlMap['6'] = SDL_SCANCODE_6;
	m_CharToSdlMap['7'] = SDL_SCANCODE_7;
	m_CharToSdlMap['8'] = SDL_SCANCODE_8;
	m_CharToSdlMap['9'] = SDL_SCANCODE_9;
}
EventManager::~EventManager()
{
	m_WinManagerPtr = nullptr;
	delete m_KeyMapNew;
	m_KeyMapNew = nullptr;
	delete m_KeyMapOld;
	m_KeyMapOld = nullptr;
	delete m_KeyMapSdl;
	m_KeyMapSdl = nullptr;
}
//----------------------------
//MemberFunctions
//----------------------------
void EventManager::Init()
{
	m_KeyMapSdl = SDL_GetKeyboardState(&m_KeyboardLength);//Set this map to be constantly updated
	m_KeyMapNew = new Uint8[m_KeyboardLength];
	m_KeyMapOld = new Uint8[m_KeyboardLength];
	m_MouseMapNew = SDL_GetMouseState(NULL, NULL);
	m_MouseMapOld = m_MouseMapNew;
}
void EventManager::UpdateEvents()
{
	std::memcpy(m_KeyMapOld, m_KeyMapNew, m_KeyboardLength);//Update Old Keyboard state
	m_MouseMapOld = m_MouseMapNew;//Update old Mouse state
	//Pump SDL events
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			m_ExitRequested = true;
			break;
		case SDL_WINDOWEVENT:
			m_WinManagerPtr->HandleWindowEvent(evnt);
			break;
		}
	}
	std::memcpy(m_KeyMapNew, m_KeyMapSdl, m_KeyboardLength);//Update New Keyboard state
	m_MouseMapNew = SDL_GetMouseState(&m_MousePosX, &m_MousePosY);//Update new Mouse state and position
}
//----------------------------
//Getters
//----------------------------
bool EventManager::IsExitRequested()
{
	return m_ExitRequested;
}
//----------------------------
//Keyboard
//----------------------------
bool EventManager::GetScancode(SDL_Scancode &code, char key)
{
	if (m_CharToSdlMap.find(key) == m_CharToSdlMap.end()) {
		return false;
	}
	else {
		code = m_CharToSdlMap[key];
		return true;
	}
}
bool EventManager::IsKeyboardKeyPressed(SDL_Scancode key)
{
	return m_KeyMapNew[key] && !m_KeyMapOld[key];
}
bool EventManager::IsKeyboardKeyPressed(char key)
{
	SDL_Scancode sKey;
	if (GetScancode(sKey, key))
	{
		return m_KeyMapNew[sKey] && !m_KeyMapOld[sKey];
	}
	return false;
}
bool EventManager::IsKeyboardKeyDown(SDL_Scancode key)
{
	return m_KeyMapNew[key] && m_KeyMapOld[key];
}
bool EventManager::IsKeyboardKeyDown(char key)
{
	SDL_Scancode sKey;
	if (GetScancode(sKey, key))
	{
		return m_KeyMapNew[sKey] && m_KeyMapOld[sKey];
	}
	return false;
}
bool EventManager::IsKeyboardKeyReleased(SDL_Scancode key)
{
	return !m_KeyMapNew[key] && m_KeyMapOld[key];
}
bool EventManager::IsKeyboardKeyReleased(char key)
{
	SDL_Scancode sKey;
	if (GetScancode(sKey, key))
	{
		return !m_KeyMapNew[sKey] && m_KeyMapOld[sKey];
	}
	return false;
}
//----------------------------
//Mouse
//----------------------------
bool EventManager::IsMouseButtonPressed(int button)
{
	return (m_MouseMapNew & SDL_BUTTON(button)) && 
		!(m_MouseMapOld & SDL_BUTTON(button));
}
bool EventManager::IsMouseButtonDown(int button)
{
	return (m_MouseMapNew & SDL_BUTTON(button)) &&
		(m_MouseMapOld & SDL_BUTTON(button));
}
bool EventManager::IsMouseButtonReleased(int button)
{
	return !(m_MouseMapNew & SDL_BUTTON(button)) &&
		(m_MouseMapOld & SDL_BUTTON(button));
}
vec2 EventManager::GetMousePosition()
{
	return vec2((float)m_MousePosX, (float)m_MousePosY);
}