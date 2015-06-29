#include "LWState.hpp"

#include "LWScene.hpp"
#include "LWSettings.hpp"

LWState::LWState(LWSettings *settingsPtr)
	:m_SettingsPtr(settingsPtr)
{
}


LWState::~LWState()
{
	delete m_RawImage;
	m_RawImage = nullptr;
	delete m_TonemappedImage;
	m_TonemappedImage = nullptr;
}

LWRenderState LWState::GetRenderState()
{
	return m_RenderState;
}

std::string LWState::GetStateString()
{
	return m_StateString;
}

std::vector<std::string> LWState::GetSceneInfo()
{
	return m_SceneInfo;
}

Texture* LWState::GetRawImage()
{
	return m_RawImage;
}

Texture* LWState::GetTonemappedImage()
{
	return m_TonemappedImage;
}

void LWState::Init()
{
	SetState(LW_STATE_SETUP);
	m_StateString = "-";
}

void LWState::SetState(LWRenderState state)
{
	m_RenderState = state;
	float duration = (std::clock() - m_RenderTime) / (float)CLOCKS_PER_SEC;
	switch (m_RenderState)
	{
	case LW_STATE_SETUP:
		m_StateString = " - ";
		break;
	case LW_STATE_PREPROCESS:
		m_RenderTime = clock();
		m_StateString = "Time: " + LW_Util::getTimeFromFloat(duration) + " || Remaining: --:-- " + " || Samples: " +
			to_string(m_SamplesRendered) + "/" + to_string(m_SettingsPtr->samples) + " || Building BVH";
		break;
	case LW_STATE_RUNNING:
		break;
	case LW_STATE_PAUSED:
		break;
	case LW_STATE_COMPLETE:
		m_StateString = "Time: " + LW_Util::getTimeFromFloat(duration) + " || Samples: " +
			to_string(m_SamplesRendered) + " || Render complete!";
		break;
	}
}
void LWState::UpdateStateString(int samplesRendered)
{
	m_SamplesRendered = samplesRendered;
	float duration = (std::clock() - m_RenderTime) / (float)CLOCKS_PER_SEC;
	float estimate = ((duration / m_SamplesRendered)*m_SettingsPtr->samples) - duration;
	m_StateString = "Time: " + LW_Util::getTimeFromFloat(duration) + " || Remaining: " + LW_Util::getTimeFromFloat(estimate) + " || Samples: " +
		to_string(m_SamplesRendered) + "/" + to_string(m_SettingsPtr->samples) + " || Pathtracing full samples";
}
void LWState::PrintPreprocessTime()
{
	float duration = (std::clock() - m_RenderTime) / (float)CLOCKS_PER_SEC;
	cout << "time for setup: " << duration << " seconds" << endl;
	cout << endl << "rendering.... " << endl;
}
void LWState::SetSceneInfo(std::vector<std::string> sceneInfo)
{
	m_SceneInfo = sceneInfo;
}