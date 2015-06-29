#pragma once
#include <iostream>

#include "TraceUnit.hpp"
#include "LWState.hpp"
#include "../../LightWhatBase/FileManagment/Import/SceneDescriptionLoader.hpp"
#include "PostProcessor.hpp"

class LWRenderer
{
public:
	LWRenderer();
	~LWRenderer();

	void Init(LWScene *scenePtr, LWSettings *settingsPtr, LWState *statePtr);
	void UpdateSettings();
	void preprocess();
	bool updateRender();//returns true if a full sample has been rendered
	void postProcessRender();
	void clear();

	//Getters
	int GetSamplesRendered();

private:
	LWScene *m_Scene = nullptr;
	LWSettings *m_SettingsPtr;
	LWState *m_StatePtr;
	//Rendering Objects
	TraceUnit *m_PathTracer = nullptr;
	PostProcessor *m_Tonemapper = nullptr;
};

