#include "ProgramController.hpp"

#include "GUI\GuiEngine.hpp"
//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GUI_ENGINE (GuiEngine::GetSingleton())

ProgramController::ProgramController(AbstractProgram *programPtr)
{
	m_ProgramPtr = programPtr;
	Init();
	EventLoop();
}
ProgramController::~ProgramController()
{
	delete m_ProgramPtr;
	m_ProgramPtr = nullptr;
}
void ProgramController::Init()
{
	GUI_ENGINE->Init();
	m_ProgramPtr->Init();
}

void ProgramController::EventLoop()
{
	//Possibly compute tick time to allow 
	//setting how many samples the Raytracer makes
	//and debugging input, tick and draw times
	while (m_Exit == false)
	{
		//Handle user input
		//Prepare cycle
		GUI_ENGINE->PreTick();
		//Tick
		if (GUI_ENGINE->IsExitRequested())
		{
			m_Exit = true;
			cout << "quitting.... " << endl;
			return;
		}
		m_ProgramPtr->Tick();
		//Paint
		GUI_ENGINE->Clear();
		m_ProgramPtr->Paint();
		GUI_ENGINE->Paint();
	}
}