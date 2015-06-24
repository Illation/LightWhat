#include "Texture.hpp"

Texture::Texture(){
}
Texture::Texture(string lName, int mX, int mY)
{
	m_Name = lName;
	m_PixelsX = mX;
	m_PixelsY = mY;

	for (size_t x = 0; x < m_PixelsX; x++)
	{
		vector<colRGB> colColumn;
		vector<float> alphaColumn;
		for (size_t y = 0; y < m_PixelsY; y++)
		{
			colColumn.push_back(colRGB(0.5, 0.5, 0.5));
			alphaColumn.push_back(1);
		}
		m_RGBcolours.push_back(colColumn);
		m_AlphaValues.push_back(alphaColumn);
	}
}
Texture::~Texture()
{
	m_AlphaValues.clear();
	m_RGBcolours.clear();
}

void Texture::setRGB(colRGB col, int x, int y){
	m_RGBcolours[x][y] = col;
}
void Texture::setAlpha(float alpha, int x, int y){
	m_AlphaValues[x][y] = alpha;
}
void Texture::setName(string lName){
	m_Name = lName;
}
void Texture::setInterpolationMode(interpolationMode lIPM){
	m_IntPolMode = lIPM;
}
void Texture::setQuadraticFittingMode(fitMode lFit){
	m_FitMode = lFit;
}

colRGB Texture::getRGB(int x, int y){
	return m_RGBcolours[x][y];
}
colRGB Texture::getRGB(float x, float y){
	float texX = 0;
	float texY = 0;
	switch (m_FitMode)
	{
	case FIT_STRETCHXY:
		texX = x*m_PixelsX;
		texY = y*m_PixelsY;
		break;
	}
	colRGB ret = colRGB(0.4f, 0.4f, 0.4f);
	switch (m_IntPolMode)
	{
	case INTPOL_PIXELS:
		ret = m_RGBcolours[(int)texX][(int)texY];
		break;
	case INTPOL_LINEAR:
		unsigned int idXL = (int)texX;
		float deltaX = texX - (idXL+0.5f);
		unsigned int idXR = idXL;
		if (deltaX>0 && idXL+1 < m_PixelsX)idXR = idXL + 1;
		else if(idXL >0)idXR = idXL - 1;

		unsigned int idYB = (int)texY;
		float deltaY = texY - (idYB+0.5f);
		unsigned int idYT = idYB;
		if (deltaY>0 && idYB + 1 < m_PixelsY)idYT = idYB + 1;
		else if(idYB>0)idYT = idYB - 1;

		colRGB bot = m_RGBcolours[idXL][idYB] * (1.f - abs(deltaX)) + m_RGBcolours[idXR][idYB] * (abs(deltaX));
		colRGB top = m_RGBcolours[idXL][idYT] * (1.f - abs(deltaX)) + m_RGBcolours[idXR][idYT] * (abs(deltaX));
		ret = bot*(1.f - abs(deltaY)) + top*abs(deltaY);
		break;
	}
	return ret;
}
float Texture::getAlpha(int x, int y){
	return m_AlphaValues[x][y];
}
float Texture::getAlpha(float x, float y){
	float texX = 0;
	float texY = 0;
	switch (m_FitMode)
	{
	case FIT_STRETCHXY:
		texX = x*m_PixelsX;
		texY = y*m_PixelsY;
		break;
	}
	float ret = 1.0;
	switch (m_IntPolMode)
	{
	case INTPOL_PIXELS:
		ret = m_AlphaValues[(int)texX][(int)texY];
		break;
	case INTPOL_LINEAR:
		unsigned int idXL = (int)texX;
		float deltaX = texX - (idXL + 0.5f);
		unsigned int idXR = idXL;
		if (deltaX>0 && idXL + 1 < m_PixelsX)idXR = idXL + 1;
		else if (idXL >0)idXR = idXL - 1;

		unsigned int idYB = (int)texY;
		float deltaY = texY - (idYB + 0.5f);
		unsigned int idYT = idYB;
		if (deltaY>0 && idYB + 1 < m_PixelsY)idYT = idYB + 1;
		else if (idYB>0)idYT = idYB - 1;

		float bot = m_AlphaValues[idXL][idYB] * (1.f - abs(deltaX)) + m_AlphaValues[idXR][idYB] * (abs(deltaX));
		float top = m_AlphaValues[idXL][idYT] * (1.f - abs(deltaX)) + m_AlphaValues[idXR][idYT] * (abs(deltaX));
		ret = bot*(1.f - abs(deltaY)) + top*abs(deltaY);
		break;
	}
	return ret;
}

size_t Texture::getWidth(){
	return m_PixelsX;
}
size_t Texture::getHeight(){
	return m_PixelsY;
}
string Texture::getName(){
	return m_Name;
}