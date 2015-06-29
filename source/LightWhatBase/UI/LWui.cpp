#include "LWui.hpp"

#include "../../UI Engine/GuiEngine.hpp"
#include "../../LightWhatRenderer/Kernel/LWRenderer.hpp"
#include "LWEventQueue.hpp"
#include "../../LightWhatRenderer/Kernel/LWSettings.hpp"
//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GUI_ENGINE (GuiEngine::GetSingleton())
//-----------------------------------------------------------------
// Constructor and Destructor
//-----------------------------------------------------------------
LWui::LWui(LWState *statePtr, LWScene *scenePtr, LWSettings *settingsPtr, LWEventQueue *evntQPtr)
	: m_StatePtr(statePtr)
	, m_ScenePtr(scenePtr)
	, m_SettingsPtr(settingsPtr)
	, m_EvntQPtr(evntQPtr)
{
	m_WinId = GUI_ENGINE->CreateNewWindow();
	m_ResolutionX = m_SettingsPtr->resolutionX;
	m_ResolutionY = m_SettingsPtr->resolutionY;
	m_ImagePosX = GUI_ENGINE->GetWidth(m_WinId) - (m_ResolutionX + 50);
	m_ImagePosY = 50;

	m_BmpResultPtr = new Bitmap(m_ResolutionX, m_ResolutionY);

	m_TexResolutionX = 400;
	m_TexResolutionY = 400;
	m_TexPosX = 50;
	m_TexPosY = GUI_ENGINE->GetHeight(m_WinId) - (m_TexResolutionX + 50);
	m_BmpTexturePtr = new Bitmap(m_TexResolutionX, m_TexResolutionY);

	m_BGRect = Rect(vec2(0, 0), vec2((float)GUI_ENGINE->GetWidth(m_WinId), (float)GUI_ENGINE->GetHeight(m_WinId)), colRGB(0.3f, 0.3f, 0.3f));

	//Load Fonts
	cout << "loading fonts...";
	m_ConsoleFontRegularPtr = new Font(GUI_ENGINE->GetRootDirectory() + string("Resources/fonts/Inconsolata-Regular.ttf"), 20, colRGB(1, 1, 1));
	m_ConsoleFontBoldPtr = new Font(GUI_ENGINE->GetRootDirectory() + string("Resources/fonts/Inconsolata-Bold.ttf"), 20, colRGB(1, 1, 1));
	cout << "    fonts loaded!" << endl;
}

LWui::~LWui()
{
	delete m_ConsoleFontRegularPtr;
	m_ConsoleFontRegularPtr = nullptr;
	delete m_ConsoleFontBoldPtr;
	m_ConsoleFontBoldPtr = nullptr;
}

void LWui::Paint()
{
	//Draw Background
	GUI_ENGINE->FillRect(m_WinId, m_BGRect);
	//Draw LWScene Info
	int x = m_ImagePosX, dy = 30, y = m_ImagePosY - 30;
	GUI_ENGINE->DrawString(m_WinId, m_StatePtr->GetStateString(), m_ConsoleFontRegularPtr, x, y);
	y = m_ImagePosY + m_ResolutionY + dy;
	for (size_t i = 0; i < m_StatePtr->GetSceneInfo().size(); i++){
		GUI_ENGINE->DrawString(m_WinId, m_StatePtr->GetSceneInfo()[i], m_ConsoleFontRegularPtr, x, y);
		y += dy;
	}
	//Draw Texture Bitmap
	if (m_DrawTexture)
	{
		GUI_ENGINE->DrawBitmap(m_WinId, m_TexPosX, m_TexPosY, m_BmpTexturePtr);
	}
	//Draw Rendered image
	GUI_ENGINE->DrawBitmap(m_WinId, m_ImagePosX, m_ImagePosY, m_BmpResultPtr);
}

void LWui::Tick()
{
	//Create a global class that manages the state and settings of the renderer, 
	//and is passed to the renderer, but usable by the UI classes,
	//and a more sophisticated event manager than a list
	if (GUI_ENGINE->IsKeyboardKeyPressed(SDL_SCANCODE_KP_ENTER) || GUI_ENGINE->IsKeyboardKeyPressed(SDL_SCANCODE_RETURN))
	{
		m_EvntQPtr->AddEvent(LW_EVENT_RENDER_START);
	}
	if (GUI_ENGINE->IsKeyboardKeyPressed('L'))
	{
		m_EvntQPtr->AddEvent(LW_EVENT_LOAD);
	}
	if (GUI_ENGINE->IsKeyboardKeyPressed('T'))
	{
		m_EvntQPtr->AddEvent(LW_EVENT_LOAD_TEST);
	}
	if (GUI_ENGINE->IsKeyboardKeyPressed('C'))
	{
		m_EvntQPtr->AddEvent(LW_EVENT_CLEAR);
	}
	if (GUI_ENGINE->IsKeyboardKeyPressed('S'))
	{
		m_EvntQPtr->AddEvent(LW_EVENT_SAVE);
	}
	if (GUI_ENGINE->IsKeyboardKeyPressed('B'))
	{
		m_EvntQPtr->AddEvent(LW_EVENT_TOGGLE_BFC);
	}
	if (GUI_ENGINE->IsKeyboardKeyPressed('D'))
	{
		m_DrawTexture = !m_DrawTexture;
		if (m_DrawTexture)
		{
			UpdateTexture();
		}
	}
	if (GUI_ENGINE->IsKeyboardKeyPressed('W'))
	{
		GUI_ENGINE->CreateNewWindow();
	}


	m_StatePtr->GetSceneInfo().clear();
	m_StatePtr->GetSceneInfo().push_back(string("LWScene info:\n"));
	m_StatePtr->GetSceneInfo().push_back(string("Vertices: ") + to_string(m_ScenePtr->vertCount) + string("\n"));
	m_StatePtr->GetSceneInfo().push_back(string("Triangles: ") + to_string(m_ScenePtr->triCount) + string("\n"));
	m_StatePtr->GetSceneInfo().push_back(string("Objects: ") + to_string(m_ScenePtr->objCount) + string("\n"));
	m_StatePtr->GetSceneInfo().push_back(string("Lights: ") + to_string(m_ScenePtr->lightCount) + string("\n"));
}

void LWui::UpdateRenderImage()
{
	for (int i = 0; i < m_ResolutionX; i++)
	{
		for (int j = 0; j < m_ResolutionY; j++)
		{
			m_BmpResultPtr->SetPixel(i, j, m_StatePtr->GetTonemappedImage()->getRGB(i, j));
		}
	}
}
//replace with bitmap functions
void LWui::UpdateTexture(){
	if (m_ScenePtr->textures.size() >0)
	{
		Texture tex = m_ScenePtr->textures[m_ActiveTextureIndex];
		for (int i = 0; i < m_TexResolutionX; i++)
		{
			for (int j = 0; j < m_TexResolutionY; j++)
			{
				m_BmpTexturePtr->SetPixel(i, j, tex.getRGB(((float)i / (float)m_TexResolutionX), ((float)j / (float)m_TexResolutionY)));
			}
		}
	}
	else
	{
		int texSizeX = 10, texSizeY = 10;
		Texture tex = Texture(string("test texture"), texSizeX, texSizeY);
		tex.setInterpolationMode(INTPOL_LINEAR);
		tex.setQuadraticFittingMode(FIT_STRETCHXY);
		for (int x = 0; x < texSizeX; x++)
		{
			for (int y = 0; y < texSizeY; y++)
			{
				tex.setRGB(colRGB(x / (float)texSizeX, y / (float)texSizeY, 1), x, y);
			}
		}
		for (int i = 0; i < m_TexResolutionX; i++)
		{
			for (int j = 0; j < m_TexResolutionY; j++)
			{
				float x = (float)i / (float)m_TexResolutionX;
				float y = (float)j / (float)m_TexResolutionY;
				colRGB thisPixel = tex.getRGB(x, y);
				m_BmpTexturePtr->SetPixel(i, j, thisPixel);
			}
		}
	}
}