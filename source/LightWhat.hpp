#pragma once
#include "AbstractProgram.hpp"

#include "Rendering/LWRenderer.hpp"
#include "GUI\LWui.hpp"

#include "FileManagment\ImageExporter.hpp"
#include "FileManagment\SceneDescriptionLoader.hpp"

enum LWEvent{
	LW_EVENT_RENDER_START,
	LW_EVENT_RENDER_PAUSE,
	LW_EVENT_RENDER_STOP,
	LW_EVENT_SAVE,
	LW_EVENT_LOAD,
	LW_EVENT_LOAD_TEST,
	LW_EVENT_CLEAR,
	LW_EVENT_TOGGLE_BFC
};

//-----------------------------------------------------------------
// LightWhat central Class
//-----------------------------------------------------------------
class LightWhat : public AbstractProgram
{
public:
	LightWhat();
	virtual ~LightWhat();

	// C++11 make the class non-copyable
	LightWhat(const LightWhat&) = delete;
	LightWhat& operator=(const LightWhat&) = delete;

	// AbstractProgram inherited functions
	virtual void Init();
	virtual void Tick();
	virtual void Paint();

	// Functions Accessible by UI

	static std::vector<LWEvent> LW_EVENT_QUEUE;
private:
	// Private functions
	void UpdateRender();
	void ProcessEventQueue();

	void StartRendering();
	void LoadFile();
	void LoadTestScene();
	void ClearScene();
	void SaveImage();
	void ToggleBfc();
	// Data members
	LWRenderer *m_Renderer = nullptr;
	ImageExporter *imgExp = nullptr;
	LWui *m_UiPtr = nullptr;
};

