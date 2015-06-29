#include "LightWhat.hpp"

#include "../UI Engine/GuiEngine.hpp"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GUI_ENGINE (GuiEngine::GetSingleton())
//-----------------------------------------------------------------
// Constructor and Destructor
//-----------------------------------------------------------------
LightWhat::LightWhat()
{
}
LightWhat::~LightWhat()
{
	delete m_Renderer;
	m_Renderer = nullptr;
	delete m_ScenePtr;
	m_ScenePtr = nullptr;
	delete m_SettingsPtr;
	m_SettingsPtr = nullptr;
	delete m_StatePtr;
	m_StatePtr = nullptr;
	delete m_ImgExportPtr;
	m_ImgExportPtr = nullptr;
	delete m_UiPtr;
	m_UiPtr = nullptr;
	delete m_EvntQueuePtr;
	m_EvntQueuePtr = nullptr;
}
//-----------------------------------------------------------------
// AbstractProgram inherited functions
//-----------------------------------------------------------------
void LightWhat::Init()
{
	m_SettingsPtr = new LWSettings();
	m_ScenePtr = new LWScene();
	m_StatePtr = new LWState(m_SettingsPtr);
	m_EvntQueuePtr = new LWEventQueue();
	m_Renderer = new LWRenderer();
	m_Renderer->Init( m_ScenePtr, m_SettingsPtr, m_StatePtr);

	LoadLWTF((GUI_ENGINE->GetRootDirectory() + string("Resources/sceneDescriptionFiles/sceneCornell.lwtf")).c_str());
	m_UiPtr = new LWui(m_StatePtr, m_ScenePtr, m_SettingsPtr, m_EvntQueuePtr);

	m_ImgExportPtr = new ImageExporter();

	//Print Info
	cout << "high performance mode" << endl;
	if (m_SettingsPtr->backfaceCulling == false){
		cout << "backface culling disabled" << endl;
	}
	else{
		cout << "backface culling enabled" << endl;
	}
}
void LightWhat::Tick()
{
	m_UiPtr->Tick();
	if (m_StatePtr->GetRenderState() == LW_STATE_RUNNING)
	{
		UpdateRender();
	}
	ProcessEventQueue();
}
void LightWhat::Paint()
{
	m_UiPtr->Paint();
}
void LightWhat::UpdateRender()
{
	if (m_Renderer->updateRender())
	{
		m_Renderer->postProcessRender();
		m_UiPtr->UpdateRenderImage();
	}
}
void LightWhat::ProcessEventQueue()
{
	while (m_EvntQueuePtr->HasEvents())
	{
		switch (m_EvntQueuePtr->PopEvent())
		{
		case LW_EVENT_RENDER_START:
			cout << endl << "starting render.... " << endl;
			StartRendering();
			break;
		case LW_EVENT_LOAD:
			LoadFile();
			break;
		case LW_EVENT_LOAD_TEST:
			LoadTestScene();
			break;
		case LW_EVENT_CLEAR:
			ClearScene();
			break;
		case LW_EVENT_SAVE:
			SaveImage();
			break;
		case LW_EVENT_TOGGLE_BFC:
			ToggleBfc();
			break;
		}
	}
}
void LightWhat::StartRendering()
{
	cout << endl << "starting render.... " << endl;
	m_Renderer->preprocess();
	m_StatePtr->PrintPreprocessTime();
}
void LightWhat::LoadFile()
{
	string fileName = GUI_ENGINE->GetRootDirectory();
	fileName = GUI_ENGINE->GetFileName();
	if ((fileName == string("nofile")))
	{
		cout << "loading file canceled " << endl;
	}
	else
	{
		cout << "loading file: " << fileName.c_str() << endl;
		LoadExternal(fileName.c_str());
		cout << "file loaded! " << endl;
	}
}
void LightWhat::LoadTestScene()
{
	cout << "loading test scene..." << endl;
	LoadLWTF((GUI_ENGINE->GetRootDirectory() + string("Resources/sceneDescriptionFiles/sceneCornell.lwtf")).c_str());
	cout << "scene loaded! " << endl;
}
void LightWhat::ClearScene()
{
	cout << "clearing scene..." << endl;
	m_Renderer->clear();
	cout << "scene cleared! " << endl;
}
void LightWhat::SaveImage()
{
	string fileName = GUI_ENGINE->GetRootDirectory();
	fileName = GUI_ENGINE->SaveFileName();
	if ((fileName == string("nofile")))
	{
		cout << "saving file canceled " << endl;
	}
	else
	{
		cout << "saving file: " << (fileName + string(".bmp")).c_str() << endl;
		m_ImgExportPtr->saveBMP((fileName + string(".bmp")).c_str(), m_SettingsPtr->resolutionX, m_SettingsPtr->resolutionY, 72, *m_StatePtr->GetTonemappedImage());
		cout << "file saved! " << endl;
	}
}
void LightWhat::ToggleBfc()
{
	if (m_SettingsPtr->backfaceCulling == false){
		cout << "backface culling enabled" << endl;
		m_SettingsPtr->backfaceCulling = true;
	}
	else{
		cout << "backface culling disabled" << endl;
		m_SettingsPtr->backfaceCulling = false;
	}
}



void LightWhat::LoadLWTF(const char* filename)
{
	//m_Scene->clearScene();
	m_ScenePtr->loadTestScene();
	SceneDescriptionLoader *LWTFloaderPtr = new SceneDescriptionLoader();
	LWTFloaderPtr->loadSceneDescriptionFile(filename, m_SettingsPtr);
	m_ScenePtr->cam = LWTFloaderPtr->cam;
	//m_Scene->materials = LWTFloader.materials;
	//m_Scene->shapes = LWTFloader.shapes;
	//m_Scene->textures = LWTFloader.textures;
	//m_Scene->lights = LWTFloader.lights;
	delete LWTFloaderPtr;
	LWTFloaderPtr = nullptr;
	m_ScenePtr->background = m_SettingsPtr->background;
	m_Renderer->UpdateSettings();
}

void LightWhat::LoadExternal(const char* filename)
{
	m_ScenePtr->loadFile(filename);
	m_ScenePtr->updateSceneInfo();
}