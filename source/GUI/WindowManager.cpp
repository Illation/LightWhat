#include "WindowManager.hpp"


WindowManager::WindowManager()
{
	_window = nullptr;
}


WindowManager::~WindowManager()
{
	//delete[] pixels;
	delete ofWnd;
	ofWnd = nullptr;
	//SDL_DestroyTexture(renderTex);
	SDL_DestroyRenderer(sdlRenderer);
}

void WindowManager::InitWindow()
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	TTF_Init();
	atexit(TTF_Quit);

	_window = SDL_CreateWindow("LightWhat - Robert Lindner", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
		, 1900, 980,
		0);

	SDL_GetWindowSize(_window, &_screenWidth, &_screenHeight);
	sdlRenderer = SDL_CreateRenderer(_window, -1, 0);
	//renderTex = SDL_CreateTexture(sdlRenderer,
	//	SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, _screenWidth, _screenHeight);
	//pixels = new Uint32[_screenWidth * _screenHeight];
	//memset(pixels, 30, _screenWidth * _screenHeight * sizeof(Uint32));

}

std::string WindowManager::GetFileName()
{
	using namespace std;
	OPENFILENAME ofn;
	char szFile[256];
	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = ofWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "3d model files (*.dae, *.3DS)\0*.dae;*.3DS";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if(GetOpenFileName(&ofn))
	return string(ofn.lpstrFile);
	else return string("nofile");
}

std::string WindowManager::GetTTFName()
{
	using namespace std;
	OPENFILENAME ofn;
	char szFile[256];
	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = ofWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Font files (*.ttf)\0*.ttf";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn))
		return string(ofn.lpstrFile);
	else return string("nofile");
}

std::string WindowManager::SaveFileName()
{
	using namespace std;
	OPENFILENAME ofn;
	char szFile[256];
	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = ofWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Bitmap Images (*.bmp)\0*.bmp\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetSaveFileName(&ofn))
		return string(ofn.lpstrFile);
	else return string("nofile");
}

void WindowManager::RenderText(const std::string &message, TTF_Font *daFont,
	SDL_Color color, int fontSize, int posX, int posY)
{
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(daFont, message.c_str(), color);
	if (surf == nullptr){
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, surf);
	if (texture == nullptr){
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);

	SDL_Rect texture_rect;
	texture_rect.x = posX;  //the x coordinate
	texture_rect.y = posY; // the y coordinate
	texture_rect.w = message.length()*fontSize / 2; //the width of the texture
	texture_rect.h = fontSize;

	SDL_RenderCopy(sdlRenderer, texture, NULL, &texture_rect);

	SDL_DestroyTexture(texture);
}

void WindowManager::UpdateWindow()
{
	//SDL_UpdateTexture(renderTex, NULL, pixels, _screenWidth *sizeof(Uint32));
	//SDL_RenderClear(sdlRenderer);
	//SDL_RenderCopy(sdlRenderer, renderTex, NULL, NULL);
	//displaySceneInfo();
	SDL_RenderPresent(sdlRenderer);
}

//void WindowManager::SetPixel(int x, int y, colRGB col)
//{
//	if (y >= 0 && y < _screenHeight && x >= 0 && x < _screenWidth)
//	{
//		int red = (int)(min(powf(col.red, (1.f / 2.2f)), 1.f) * 255) << 16;
//		int green = (int)(min(powf(col.green, (1.f / 2.2f)), 1.f) * 255) << 8;
//		int blue = (int)(min(powf(col.blue, (1.f / 2.2f)), 1.f) * 255);
//		Uint32 uint = Uint32(red + green + blue);
//		pixels[y * _screenWidth + x] = uint;
//	}
//}

int WindowManager::GetWindowHeight()
{
	return _screenHeight;
}

int WindowManager::GetWindowWidth()
{
	return _screenWidth;
}

SDL_Renderer* WindowManager::GetRenderer()
{
	return sdlRenderer;
}