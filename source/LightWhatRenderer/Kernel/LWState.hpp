#pragma once

#include "../Utility/Texture.hpp"
#include <ctime>
//class foreward declaration
class LWSettings;
class LWScene;
enum LWRenderState
{
	LW_STATE_SETUP,
	LW_STATE_PREPROCESS,
	LW_STATE_RUNNING,
	LW_STATE_PAUSED,
	LW_STATE_COMPLETE
};
class LWState
{
	friend class LWRenderer;
public:
	LWState(LWSettings *settingsPtr);
	virtual ~LWState();

	void PrintPreprocessTime();

	LWRenderState GetRenderState();
	std::string GetStateString();
	std::vector<std::string> GetSceneInfo();

	Texture* GetRawImage();
	Texture* GetTonemappedImage();
protected:
	void Init();
	void SetState(LWRenderState state);
	void UpdateStateString(int samplesRendered);
	void SetSceneInfo(std::vector<std::string> sceneInfo);
private:
	//Data Members
	std::string m_StateString = "-";
	std::vector<std::string> m_SceneInfo;

	LWRenderState m_RenderState = LW_STATE_SETUP;

	Texture *m_RawImage = nullptr;
	Texture *m_TonemappedImage = nullptr;

	LWSettings *m_SettingsPtr = nullptr;
	LWScene *m_ScenePtr = nullptr;

	clock_t m_RenderTime;
	int m_SamplesRendered = 0;
};

