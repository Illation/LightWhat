#include "LWRenderer.hpp"


LWRenderer::LWRenderer()
{
}


LWRenderer::~LWRenderer()
{
	delete m_Scene;
	m_Scene = nullptr;
	delete m_Tonemapper;
	m_Tonemapper = nullptr;
	delete m_PathTracer;
	m_PathTracer = nullptr;
}

void LWRenderer::init(const char* defaultFile)
{
	m_Status = "-";
	m_Scene = new Scene();
	m_PathTracer = new TraceUnit();
	m_Tonemapper = new PostProcessor();
	m_PathTracer->setScene(m_Scene);

	loadLWTF(defaultFile);
	m_RenderStatus = LW_SETUP;
}

void LWRenderer::loadLWTF(const char* filename)
{
	m_Status = "-";
	//m_Scene->clearScene();
	m_Scene->loadTestScene();
	SceneDescriptionLoader LWTFloader;
	LWTFloader.loadSceneDescriptionFile(filename);
	m_Settings = LWTFloader.settings;
	m_Scene->cam = LWTFloader.cam;
	//m_Scene->materials = LWTFloader.materials;
	//m_Scene->shapes = LWTFloader.shapes;
	//m_Scene->textures = LWTFloader.textures;
	//m_Scene->lights = LWTFloader.lights;
	m_Scene->background = m_Settings.background;

	m_PathTracer->m_BackfaceCulling = m_Settings.backfaceCulling;
	m_PathTracer->p_MaxBounces = m_Settings.bounces;
	m_RenderStatus = LW_PREPROCESS;
}

void LWRenderer::importExternal(const char* filename)
{
	m_Scene->loadFile(filename);
	m_Scene->updateSceneInfo();
	m_RenderStatus = LW_PREPROCESS;
}

void LWRenderer::preprocess()
{
	m_RenderTime = clock(); 
	float duration = (std::clock() - m_RenderTime) / (float)CLOCKS_PER_SEC;
	m_Status = "Time: " + LW_Util::getTimeFromFloat(duration) + " || Remaining: --:-- " + " || Samples: " +
		to_string(m_PathTracer->m_samplesRendered) + "/" + to_string(m_Settings.samples) + " || Building BVH";
	m_PathTracer->init(m_Settings.resolutionX, m_Settings.resolutionY);
	m_RawImage = m_PathTracer->getImage();
	m_TonemappedImage = m_RawImage;
	duration = (std::clock() - m_RenderTime) / (float)CLOCKS_PER_SEC;
	cout << "time for setup: " << duration << " seconds" << endl;
	cout << endl << "rendering.... " << endl;
	m_RenderStatus = LW_RUNNING;
}

bool LWRenderer::updateRender()
{
	if (m_PathTracer->renderNextChunk())
	{
		if (m_PathTracer->m_samplesRendered >= m_Settings.samples)
		{
			m_RenderStatus = LW_COMPLETE;
			float duration = (std::clock() - m_RenderTime) / (float)CLOCKS_PER_SEC;
			m_Status = "Time: " + LW_Util::getTimeFromFloat(duration) + " || Samples: " +
				to_string(m_PathTracer->m_samplesRendered)+ " || Render complete!";
			m_PathTracer->m_samplesRendered = 0;
		}
		else
		{
			m_RenderStatus = LW_RUNNING;
			m_PathTracer->updateRayMap();
			float duration = (std::clock() - m_RenderTime) / (float)CLOCKS_PER_SEC;
			float estimate = ((duration / m_PathTracer->m_samplesRendered)*m_Settings.samples) - duration;
			m_Status = "Time: " + LW_Util::getTimeFromFloat(duration) + " || Remaining: " + LW_Util::getTimeFromFloat(estimate) + " || Samples: " +
				to_string(m_PathTracer->m_samplesRendered) + "/" + to_string(m_Settings.samples) + " || Pathtracing full samples";
		}
		return true;
	}
	return false;
}

void LWRenderer::postProcessRender()
{
	m_Tonemapper->updateHighestExposure(*m_RawImage, m_Settings.resolutionX, m_Settings.resolutionY);
	m_Tonemapper->controlExposure(*m_RawImage, *m_TonemappedImage, m_Settings.resolutionX, m_Settings.resolutionY, m_Settings.toneMapping);
}

void LWRenderer::clear()
{
	m_Scene->clearScene();
}