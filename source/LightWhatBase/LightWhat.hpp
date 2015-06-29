#pragma once
#include "../../UI Engine/ProgramControl/AbstractProgram.hpp"

#include "../LightWhatRenderer/Kernel/LWRenderer.hpp"
#include "UI\LWui.hpp"
#include "UI\LWEventQueue.hpp"

#include "FileManagment\Export/ImageExporter.hpp"
#include "FileManagment\Import/SceneDescriptionLoader.hpp"

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

	void LoadLWTF(const char *filename);
	void LoadExternal(const char *filename);
	// Data members
	LWRenderer *m_Renderer = nullptr;
	LWState *m_StatePtr = nullptr;
	LWSettings *m_SettingsPtr = nullptr;
	LWScene *m_ScenePtr = nullptr;

	LWEventQueue *m_EvntQueuePtr = nullptr;
	LWui *m_UiPtr = nullptr;

	ImageExporter *m_ImgExportPtr = nullptr;
};

