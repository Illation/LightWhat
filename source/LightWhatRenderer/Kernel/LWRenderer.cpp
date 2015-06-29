#include "LWRenderer.hpp"


LWRenderer::LWRenderer()
{
}


LWRenderer::~LWRenderer()
{
	delete m_Tonemapper;
	m_Tonemapper = nullptr;
	delete m_PathTracer;
	m_PathTracer = nullptr;
}

void LWRenderer::Init(LWScene *scenePtr, LWSettings *settingsPtr, LWState *statePtr)
{
	m_SettingsPtr = settingsPtr;
	m_Scene = scenePtr;
	m_StatePtr = statePtr;
	m_PathTracer = new TraceUnit();
	m_Tonemapper = new PostProcessor();
	m_PathTracer->setScene(m_Scene);

	m_StatePtr->Init();
}

void LWRenderer::UpdateSettings()
{
	m_PathTracer->m_BackfaceCulling = m_SettingsPtr->backfaceCulling;
	m_PathTracer->p_MaxBounces = m_SettingsPtr->bounces;
}

void LWRenderer::preprocess()
{
	m_StatePtr->m_RawImage = new Texture("Render result", m_SettingsPtr->resolutionX, m_SettingsPtr->resolutionY);
	m_StatePtr->m_RawImage->setInterpolationMode(INTPOL_PIXELS);
	m_StatePtr->m_RawImage->setQuadraticFittingMode(FIT_STRETCHXY);

	m_StatePtr->m_TonemappedImage = new Texture("Render result", m_SettingsPtr->resolutionX, m_SettingsPtr->resolutionY);
	m_StatePtr->m_TonemappedImage->setInterpolationMode(INTPOL_PIXELS);
	m_StatePtr->m_TonemappedImage->setQuadraticFittingMode(FIT_STRETCHXY);

	m_StatePtr->SetState(LW_STATE_PREPROCESS);
	m_PathTracer->Init(m_StatePtr->m_RawImage, m_SettingsPtr->resolutionX, m_SettingsPtr->resolutionY);

	m_StatePtr->SetState(LW_STATE_RUNNING);
}

bool LWRenderer::updateRender()
{
	if (m_PathTracer->renderNextChunk())
	{
		if (m_PathTracer->m_samplesRendered >= m_SettingsPtr->samples)
		{
			m_StatePtr->m_SamplesRendered = m_PathTracer->m_samplesRendered;
			m_PathTracer->m_samplesRendered = 0;
			m_StatePtr->SetState(LW_STATE_COMPLETE);
		}
		else
		{
			m_PathTracer->updateRayMap();
			m_StatePtr->UpdateStateString(m_PathTracer->m_samplesRendered);
		}
		return true;
	}
	return false;
}

void LWRenderer::postProcessRender()
{
	m_Tonemapper->updateHighestExposure(*m_StatePtr->m_RawImage, m_SettingsPtr->resolutionX, m_SettingsPtr->resolutionY);
	m_Tonemapper->controlExposure(*m_StatePtr->m_RawImage, *m_StatePtr->m_TonemappedImage, m_SettingsPtr->resolutionX, m_SettingsPtr->resolutionY, m_SettingsPtr->toneMapping);
}

void LWRenderer::clear()
{
	m_Scene->clearScene();
}