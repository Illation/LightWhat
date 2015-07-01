#pragma once

#include "../../UI Engine/Components/Button.hpp"
#include "../../UI Engine/Components/Bitmap.hpp"
#include "../../UI Engine/Components/Font.hpp"
#include "../../UI Engine/Components/Rectangle.hpp"

//Foreward Declaration
class LWEventQueue;
struct LWSettings;
class LWState;
class LWScene;
//-----------------------------------------------------------------
// LWui Class
//-----------------------------------------------------------------
class LWui
{
public:
	LWui(LWState* statePtr, LWScene* scenePtr, LWSettings* settingsPtr, LWEventQueue* evntQPtr);
	~LWui();

	// C++11 make the class non-copyable
	LWui(const LWui&) = delete;
	LWui& operator=(const LWui&) = delete;

	void Tick();
	void Paint();
	void UpdateRenderImage();

private:
	//Member functions
	void UpdateTexture();

	//Data members
	Bitmap *m_BmpResultPtr = nullptr;
	Bitmap *m_BmpTexturePtr = nullptr;

	Button *m_BtnRenderPtr = nullptr
		, *m_BtnLoadPtr = nullptr
		, *m_BtnLoadTestPtr = nullptr
		, *m_BtnClearPtr = nullptr
		, *m_BtnBfcPtr = nullptr
		, *m_BtnSavePtr = nullptr;

	LWState *m_StatePtr = nullptr;
	LWSettings *m_SettingsPtr = nullptr;
	LWEventQueue *m_EvntQPtr = nullptr;
	LWScene *m_ScenePtr = nullptr;

	//Strings
	Font *m_ConsoleFontRegularPtr = nullptr;
	Font *m_ConsoleFontBoldPtr = nullptr;

	//Window
	int m_WinId;

	//Layout parameters

	Rect m_BGRect;

	int m_ImagePosX;
	int m_ImagePosY;
	int m_ResolutionX;
	int m_ResolutionY;

	int m_TexPosX;
	int m_TexPosY;
	int m_TexResolutionX;
	int m_TexResolutionY;
	bool m_DrawTexture = false;

	size_t m_ActiveTextureIndex = 0;
};

