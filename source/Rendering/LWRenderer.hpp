#pragma once
#include <iostream>

#include "TraceUnit.hpp"
#include "../FileManagment\SceneDescriptionLoader.hpp"
#include "PostProcessor.hpp"

enum LWRenderStatus
{
	LW_SETUP,
	LW_PREPROCESS,
	LW_RUNNING,
	LW_PAUSED,
	LW_COMPLETE
};
class LWRenderer
{
public:
	LWRenderer();
	~LWRenderer();

	void init(const char *defaultFile);
	void loadLWTF(const char *filename);
	void importExternal(const char *filename);
	void preprocess();
	bool updateRender();//returns true if a full sample has been rendered
	void postProcessRender();
	void clear();

	//Rendering Objects
	Scene *m_Scene = nullptr;
	TraceUnit *m_PathTracer = nullptr;
	PostProcessor *m_Tonemapper = nullptr;
	Texture *m_RawImage = nullptr;
	Texture *m_TonemappedImage = nullptr;

	//Utility Objects
	string m_Status = "-";
	LWRenderStatus m_RenderStatus = LW_SETUP;
	renderSettings m_Settings;

private:
	clock_t m_RenderTime;
};

