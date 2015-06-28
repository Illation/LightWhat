#pragma once
#include "../Rendering/LWRenderer.hpp"

#include "Components/Bitmap.hpp"
#include "Components/Font.hpp"
#include "Components/Rectangle.hpp"

//-----------------------------------------------------------------
// LWui Class
//-----------------------------------------------------------------
class LWui
{
public:
	LWui(LWRenderer *rendererPtr);
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
	LWRenderer *m_RendererPtr = nullptr;

	//Strings
	std::vector<std::string> m_SceneInfo;
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

