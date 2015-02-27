#pragma once
#include "Scene.h"
#include "commonR.h"
class Renderer
{
public:				
	Renderer();
	virtual ~Renderer();

	bool renderNextRow();
	std::vector<vector<colRGB> >getImage();
	void setScene(Scene *sc);
	void init(int camWidth, int camHeight);

	colRGB raycast(Ray);
	bool shadowRay(line);
	colRGB shade(DifferentialGeometry);

	const int p_MaxBounces = 8;
	bool m_BackfaceCulling = false;
private:

	vector<vector<colRGB> > m_Image;
	vector<vector<Ray> > rayMap;
	Scene *m_ScenePtr = nullptr;

	int m_currentRow = 0;
	int m_samplesRendered = 0;

	Renderer(const Renderer& tRef);
	Renderer& operator=(const Renderer& tRef);

};
