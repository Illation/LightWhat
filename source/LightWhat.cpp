#include "LightWhat.hpp"

#include "GUI\GuiEngine.hpp"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GUI_ENGINE (GuiEngine::GetSingleton())
//-----------------------------------------------------------------
// Static Initialization
//-----------------------------------------------------------------
std::vector<LWEvent> LightWhat::LW_EVENT_QUEUE = {};
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
	delete imgExp;
	imgExp = nullptr;
	delete m_UiPtr;
	m_UiPtr = nullptr;
}
//-----------------------------------------------------------------
// AbstractProgram inherited functions
//-----------------------------------------------------------------
void LightWhat::Init()
{
	m_Renderer = new LWRenderer();
	m_Renderer->init((GUI_ENGINE->GetRootDirectory() + string("Resources/sceneDescriptionFiles/sceneCornell.lwtf")).c_str());
	m_UiPtr = new LWui(m_Renderer);

	imgExp = new ImageExporter();

	//Print Info
	cout << "high performance mode" << endl;
	if (m_Renderer->m_Settings.backfaceCulling == false){
		cout << "backface culling disabled" << endl;
	}
	else{
		cout << "backface culling enabled" << endl;
	}
}
void LightWhat::Tick()
{
	m_UiPtr->Tick();
	if (m_Renderer->m_RenderStatus == LW_RUNNING)
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
		m_UiPtr->UpdateRenderImage();
	}
}
void LightWhat::ProcessEventQueue()
{
	while (LW_EVENT_QUEUE.size() > 0)
	{
		switch (LW_EVENT_QUEUE[0])
		{
		case LW_EVENT_RENDER_START:
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
		LW_EVENT_QUEUE.erase(LW_EVENT_QUEUE.begin() + 0);
	}
}
void LightWhat::StartRendering()
{
	cout << endl << "starting render.... " << endl;
	m_Renderer->preprocess();
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
		m_Renderer->importExternal(fileName.c_str());
		cout << "file loaded! " << endl;
	}
}
void LightWhat::LoadTestScene()
{
	cout << "loading test scene..." << endl;
	m_Renderer->loadLWTF((GUI_ENGINE->GetRootDirectory() + string("Resources/sceneDescriptionFiles/sceneCornell.lwtf")).c_str());
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
		imgExp->saveBMP((fileName + string(".bmp")).c_str(), m_Renderer->m_Settings.resolutionX, m_Renderer->m_Settings.resolutionY, 72, *m_Renderer->m_TonemappedImage);
		cout << "file saved! " << endl;
	}
}
void LightWhat::ToggleBfc()
{
	if (m_Renderer->m_Settings.backfaceCulling == false){
		cout << "backface culling enabled" << endl;
		m_Renderer->m_Settings.backfaceCulling = true;
	}
	else{
		cout << "backface culling disabled" << endl;
		m_Renderer->m_Settings.backfaceCulling = false;
	}
}