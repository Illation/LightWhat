#include "GuiEngine.hpp"
//----------------------------
//Static Variable
//----------------------------
GuiEngine* GuiEngine::m_GuiEnginePtr = nullptr;
//----------------------------
//Constructor and Destructor
//----------------------------
GuiEngine::GuiEngine()
{
	m_WindowsWindow = nullptr;
	m_WindowPtr = new WindowManager();
	m_EventMngPtr = new EventManager(m_WindowPtr);

}
GuiEngine::~GuiEngine()
{
	delete m_WindowsWindow;
	m_WindowsWindow = nullptr;
	delete m_EventMngPtr;
	m_EventMngPtr = nullptr;
	delete m_WindowPtr;
	m_WindowPtr = nullptr;
}
//----------------------------
//Static Engine Functions
//----------------------------
GuiEngine* GuiEngine::GetSingleton()
{
	if (m_GuiEnginePtr == nullptr)
	{
		m_GuiEnginePtr = new GuiEngine();
	}
	return m_GuiEnginePtr;
}
//----------------------------
//Windows Functions
//----------------------------
std::string GuiEngine::GetFileName()
{
	using namespace std;
	OPENFILENAME ofn;
	char szFile[256];
	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_WindowsWindow;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "3d model files (*.dae, *.3DS)\0*.dae;*.3DS";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn))
		return string(ofn.lpstrFile);
	else return string("nofile");
}

std::string GuiEngine::GetTTFName()
{
	using namespace std;
	OPENFILENAME ofn;
	char szFile[256];
	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_WindowsWindow;
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

std::string GuiEngine::GetRootDirectory()
{
	return m_RootDirectory;
}

std::string GuiEngine::SaveFileName()
{
	using namespace std;
	OPENFILENAME ofn;
	char szFile[256];
	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_WindowsWindow;
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
//----------------------------
//Application Methods
//----------------------------
void GuiEngine::Init()
{
	cout << "Initializing Engine ..." << endl;

	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	TTF_Init();
	atexit(TTF_Quit);

	//Init the image Loader
	ilInit();
	iluInit();
	ilutRenderer(ILUT_DIRECT3D9);
	//Initialize Managers
	m_EventMngPtr->Init();

	//Get Root dir
	cout << "Loading root directory..." << endl;
	char ownPth[MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule == NULL)
	{
		cout << "Failed to load root directory, quitting..." << endl;
		m_Exit = true;
		cout << "Press Enter to Continue";
		cin.get();
	}
	else
	{
		GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));

		std::string t = ownPth;
		std::string s = "LightWhat.exe";

		std::string::size_type i = t.find(s);

		if (i != std::string::npos)
			t.erase(i, s.length());

		m_RootDirectory = t;
	}
}
void GuiEngine::Clear()
{
	m_WindowPtr->ClearWindows();
}
void GuiEngine::PreTick()
{
	m_EventMngPtr->UpdateEvents();
}
void GuiEngine::Paint()
{
	m_WindowPtr->UpdateWindows();
}
//----------------------------
//Window Methods
//----------------------------
int GuiEngine::CreateNewWindow()
{
	return m_WindowPtr->CreateNewWindow();
}
void GuiEngine::DestroyWindow(int windowId)
{
	m_WindowPtr->DestroyWindow(windowId);
}
//----------------------------
//General Getters
//----------------------------
WindowManager* GuiEngine::GetWindow()
{
	return m_WindowPtr;
}
int GuiEngine::GetWidth(int windowId)
{
	return m_WindowPtr->GetWindow(windowId)->GetWidth();
}
int GuiEngine::GetHeight(int windowId)
{
	return m_WindowPtr->GetWindow(windowId)->GetHeight();
}
bool GuiEngine::IsExitRequested()
{
	return m_EventMngPtr->IsExitRequested()||m_WindowPtr->AllWindowsClosed()||m_Exit;
}
//----------------------------
//Draw Methods
//----------------------------
void GuiEngine::DrawBitmap(int windowId, int x, int y, Bitmap *bmpPtr)
{
	SDL_Rect texture_rect;
	texture_rect.x = x;
	texture_rect.y = y;
	texture_rect.w = bmpPtr->GetWidth();
	texture_rect.h = bmpPtr->GetHeight();

	SDL_Texture* texture = SDL_CreateTexture(m_WindowPtr->GetWindow(windowId)->GetRenderer(),
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 
		bmpPtr->GetWidth(), bmpPtr->GetHeight());
	SDL_UpdateTexture(texture, NULL, bmpPtr->GetData(), bmpPtr->GetWidth() *sizeof(Uint32));

	SDL_RenderCopy(m_WindowPtr->GetWindow(windowId)->GetRenderer(), texture, NULL, &texture_rect);
	SDL_DestroyTexture(texture);
}

void GuiEngine::DrawString(int windowId, const std::string &message, Font *fontPtr, int posX, int posY)
{
	SDL_Surface *surf = TTF_RenderText_Blended(fontPtr->sdlFont, message.c_str(), fontPtr->color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(m_WindowPtr->GetWindow(windowId)->GetRenderer(), surf);
	SDL_FreeSurface(surf);

	SDL_Rect texture_rect;
	texture_rect.x = posX;
	texture_rect.y = posY;
	texture_rect.w = message.length()*fontPtr->size / 2;
	texture_rect.h = fontPtr->size;

	SDL_RenderCopy(m_WindowPtr->GetWindow(windowId)->GetRenderer(), texture, NULL, &texture_rect);
	SDL_DestroyTexture(texture);
}

void GuiEngine::FillRect(int windowId, Rect rect)
{
	SDL_Surface *surf = SDL_CreateRGBSurface(0, (int)rect.dim.x, (int)rect.dim.y, 32, 0, 0, 0, 0);
	SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, (Uint8)(rect.col.red * 255), (Uint8)(rect.col.green * 255), (Uint8)(rect.col.blue * 255)));
	SDL_Texture *texture = SDL_CreateTextureFromSurface(m_WindowPtr->GetWindow(windowId)->GetRenderer(), surf);
	SDL_FreeSurface(surf);

	SDL_Rect texture_rect;
	texture_rect.x = (int)rect.pos.x;
	texture_rect.y = (int)rect.pos.y;
	texture_rect.w = (int)rect.dim.x;
	texture_rect.h = (int)rect.dim.y;

	SDL_RenderCopy(m_WindowPtr->GetWindow(windowId)->GetRenderer(), texture, NULL, &texture_rect);
	SDL_DestroyTexture(texture);
}
//----------------------------
// Input
//----------------------------
bool GuiEngine::IsKeyboardKeyPressed(SDL_Scancode key)
{
	return m_EventMngPtr->IsKeyboardKeyPressed(key);
}
bool GuiEngine::IsKeyboardKeyPressed(char key)
{
	return m_EventMngPtr->IsKeyboardKeyPressed(key);
}

bool GuiEngine::IsKeyboardKeyDown(SDL_Scancode key)
{
	return m_EventMngPtr->IsKeyboardKeyDown(key);
}
bool GuiEngine::IsKeyboardKeyDown(char key)
{
	return m_EventMngPtr->IsKeyboardKeyDown(key);
}

bool GuiEngine::IsKeyboardKeyReleased(SDL_Scancode key)
{
	return m_EventMngPtr->IsKeyboardKeyReleased(key);
}
bool GuiEngine::IsKeyboardKeyReleased(char key)
{
	return m_EventMngPtr->IsKeyboardKeyReleased(key);
}