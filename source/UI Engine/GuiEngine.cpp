#include "GuiEngine.hpp"

#include <Windows.h>
#include <Commdlg.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
//----------------------------
//Static Variable
//----------------------------
GuiEngine* GuiEngine::m_GuiEnginePtr = nullptr;
//----------------------------
//Constructor and Destructor
//----------------------------
GuiEngine::GuiEngine()
{
	m_WindowPtr = new WindowManager();
	m_EventMngPtr = new EventManager(m_WindowPtr);

}
GuiEngine::~GuiEngine()
{
	delete m_EventMngPtr;
	m_EventMngPtr = nullptr;
	delete m_WindowPtr;
	m_WindowPtr = nullptr;
	delete m_DefaultFontPtr;
	m_DefaultFontPtr = nullptr;
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
	HWND m_WindowsWindow = nullptr;
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
	HWND m_WindowsWindow = nullptr;
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
	HWND m_WindowsWindow = nullptr;
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
		cout << "Press ENTER to continue...";
		cin.get();
		return;
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
	m_DefaultFontPtr = new Font(GetRootDirectory() + string("Resources/fonts/Inconsolata-Regular.ttf"), 20);
	m_FontPtr = m_DefaultFontPtr;
	m_Color = { 255, 255, 255, 255 };
}
void GuiEngine::Clear()
{
	m_WindowPtr->ClearWindows();
}
void GuiEngine::PreTick()
{
	m_EventMngPtr->UpdateEvents();
	//Tick all GuiObjects
	for (size_t i = 0; i < m_GuiObjPtrArr.size(); i++)
	{
		m_GuiObjPtrArr[i]->Tick();
	}
}
void GuiEngine::Paint()
{
	//Consume all GuiObject Events
	for (size_t i = 0; i < m_GuiObjPtrArr.size(); i++)
	{
		m_GuiObjPtrArr[i]->ConsumeEvent();
	}
	//Paint windows
	m_WindowPtr->UpdateWindows();
}
bool GuiEngine::IsExitRequested()
{
	return m_EventMngPtr->IsExitRequested()||m_WindowPtr->AllWindowsClosed()||m_Exit;
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
int GuiEngine::GetWidth(int windowId)
{
	return m_WindowPtr->GetWindow(windowId)->GetWidth();
}
int GuiEngine::GetHeight(int windowId)
{
	return m_WindowPtr->GetWindow(windowId)->GetHeight();
}
//----------------------------
//Setters for Drawing
//----------------------------
void GuiEngine::SetWindow(int windowId)
{
	if (m_WindowPtr->HasWindow(windowId))
	{
		m_CurrentWindowId = windowId;
	}
	else
	{
		cout << "no window with id: '" << to_string(windowId) << "' found" << endl;
	}
}
void GuiEngine::SetColor(colRGB c)
{
	m_Color = { (int)(c.red * 255), (int)(c.green * 255), (int)(c.blue * 255), 255 };
}
void GuiEngine::SetFont(Font *fontPtr)
{
	m_FontPtr = fontPtr;
}
//----------------------------
//Getters for Drawing
//----------------------------
Font* GuiEngine::GetFont()
{
	return m_FontPtr;
}
//----------------------------
//Draw Methods
//----------------------------
void GuiEngine::DrawBitmap(int x, int y, Bitmap *bmpPtr)
{
	SDL_Rect texture_rect;
	texture_rect.x = x;
	texture_rect.y = y;
	texture_rect.w = bmpPtr->GetWidth();
	texture_rect.h = bmpPtr->GetHeight();

	SDL_Texture* texture = SDL_CreateTexture(m_WindowPtr->GetWindow(m_CurrentWindowId)->GetRenderer(),
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 
		bmpPtr->GetWidth(), bmpPtr->GetHeight());
	SDL_UpdateTexture(texture, NULL, bmpPtr->GetData(), bmpPtr->GetWidth() *sizeof(Uint32));

	SDL_RenderCopy(m_WindowPtr->GetWindow(m_CurrentWindowId)->GetRenderer(), texture, NULL, &texture_rect);
	SDL_DestroyTexture(texture);
}

void GuiEngine::DrawString(const std::string &message, int posX, int posY)
{
	SDL_Surface *surf = TTF_RenderText_Blended(m_FontPtr->sdlFont, message.c_str(), m_Color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(m_WindowPtr->GetWindow(m_CurrentWindowId)->GetRenderer(), surf);
	SDL_FreeSurface(surf);

	SDL_Rect texture_rect;
	texture_rect.x = posX;
	texture_rect.y = posY;
	texture_rect.w = message.length()*m_FontPtr->size / 2;
	texture_rect.h = m_FontPtr->size;

	SDL_RenderCopy(m_WindowPtr->GetWindow(m_CurrentWindowId)->GetRenderer(), texture, NULL, &texture_rect);
	SDL_DestroyTexture(texture);
}

void GuiEngine::DrawLine(vec2 pos1, vec2 pos2)
{
	SDL_SetRenderDrawColor(m_WindowPtr->GetWindow(m_CurrentWindowId)->GetRenderer(),
		m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	SDL_RenderDrawLine(m_WindowPtr->GetWindow(m_CurrentWindowId)->GetRenderer(),
		(int)pos1.x, (int)pos1.y, (int)pos2.x, (int)pos2.y);
}

void GuiEngine::DrawRect(Rect rect)
{
	SDL_Rect outlineRect = { (int)rect.pos.x, (int)rect.pos.y, (int)rect.dim.x, (int)rect.dim.y };
	SDL_SetRenderDrawColor(m_WindowPtr->GetWindow(m_CurrentWindowId)->GetRenderer(),
		m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	SDL_RenderDrawRect(m_WindowPtr->GetWindow(m_CurrentWindowId)->GetRenderer(),
		&outlineRect);
}

void GuiEngine::FillRect(Rect rect)
{
	SDL_Rect outlineRect = { (int)rect.pos.x, (int)rect.pos.y, (int)rect.dim.x, (int)rect.dim.y };
	SDL_SetRenderDrawColor(m_WindowPtr->GetWindow(m_CurrentWindowId)->GetRenderer(),
		m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	SDL_RenderFillRect(m_WindowPtr->GetWindow(m_CurrentWindowId)->GetRenderer(),
		&outlineRect);
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

bool GuiEngine::IsMouseButtonPressed(int button)
{
	return m_EventMngPtr->IsMouseButtonPressed(button);
}
bool GuiEngine::IsMouseButtonDown(int button)
{
	return m_EventMngPtr->IsMouseButtonDown(button);
}
bool GuiEngine::IsMouseButtonReleased(int button)
{
	return m_EventMngPtr->IsMouseButtonReleased(button);
}
vec2 GuiEngine::GetMousePosition()
{
	return m_EventMngPtr->GetMousePosition();
}
//----------------------------
// Gui Objects
//----------------------------
void GuiEngine::RegisterGuiObject(GuiObject *objPtr)
{
	m_GuiObjPtrArr.push_back(objPtr);
}
void GuiEngine::UnRegisterGuiObject(GuiObject *objPtr)
{
	std::vector<GuiObject*>::iterator pos = find(m_GuiObjPtrArr.begin(), m_GuiObjPtrArr.end(), objPtr); // find algorithm from STL

	if (pos == m_GuiObjPtrArr.end()) return;
	else
	{
		m_GuiObjPtrArr.erase(pos);
		return;
	}
	m_GuiObjPtrArr.push_back(objPtr);
}