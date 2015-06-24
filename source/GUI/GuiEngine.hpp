#pragma once
#include "WindowManager.hpp"

#include "Components\Bitmap.hpp"
class GuiEngine
{
public:
	virtual ~GuiEngine();
	static GuiEngine* GetSingleton();//duh

	//Application methods
	void Init();
	void Paint();

	//General getters
	WindowManager* GetWindow();
	int GetWidth();
	int GetHeight();

	//Draw methods
	void DrawBitmap(int x, int y, Bitmap *bmpPtr);
	void DrawString(const std::string &message, TTF_Font *daFont,
		SDL_Color color, int fontSize, int posX, int posY);
private:
	//Singleton constructor and instance
	GuiEngine();
	static GuiEngine* m_GuiEnginePtr;

	WindowManager *m_WindowPtr = nullptr;
	std::vector<SDL_Texture*> m_TexturePtrArr;
	std::vector<SDL_Rect> m_RectArr;
};

