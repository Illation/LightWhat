#include "Texture.hpp"

Texture::Texture(){
}
Texture::Texture(string lName, int mX, int mY)
{
	m_Name = lName;
	m_Width = mX;
	m_Height = mY;
	m_Length = mX*mY * 4;
	m_Pixels = new float[m_Length];
	memset(m_Pixels, 0.5f, m_Length);
}
Texture::~Texture()
{
}

void Texture::setRGB(colRGB col, int x, int y){
	int idxBase = (x + y*m_Width)*4;
	m_Pixels[(idxBase)] = col.red;
	m_Pixels[(idxBase+1)] = col.green;
	m_Pixels[(idxBase+2)] = col.blue;
}
void Texture::setAlpha(float alpha, int x, int y){
	int idxBase = (x + y*m_Width) * 4;
	m_Pixels[(idxBase+3)] = alpha;
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
	int idxBase = (x + y*m_Width) * 4;
	return colRGB(m_Pixels[idxBase], m_Pixels[idxBase + 1], m_Pixels[idxBase+2]);
}
colRGB Texture::getRGB(float x, float y){
	float texX = 0;
	float texY = 0;
	switch (m_FitMode)
	{
	case FIT_STRETCHXY:
		texX = x*m_Width;
		texY = y*m_Height;
		break;
	}
	colRGB ret = colRGB(0.5, 0.5, 0.5);
	switch (m_IntPolMode)
	{
	case INTPOL_PIXELS:
		ret = getRGB((int)texX, (int)texY);
		break;
	case INTPOL_LINEAR:
		int idXL = (int)texX;
		float deltaX = texX - (idXL+0.5f);
		int idXR = idXL;
		if (deltaX>0 && idXL+1 < m_Width)idXR = idXL + 1;
		else if(idXL >0)idXR = idXL - 1;

		int idYB = (int)texY;
		float deltaY = texY - (idYB+0.5f);
		int idYT = idYB;
		if (deltaY>0 && idYB + 1 < m_Height)idYT = idYB + 1;
		else if(idYB>0)idYT = idYB - 1;

		colRGB bot = getRGB(idXL, idYB) * (1.f - abs(deltaX)) + getRGB(idXR, idYB) * (abs(deltaX));
		colRGB top = getRGB(idXL, idYT) * (1.f - abs(deltaX)) + getRGB(idXR, idYT) * (abs(deltaX));
		ret = bot*(1.f - abs(deltaY)) + top*abs(deltaY);
		break;
	}
	return ret;
}
float Texture::getAlpha(int x, int y){
	return m_Pixels[(x + y*m_Width) * 4 + 3];
}
float Texture::getAlpha(float x, float y){
	float texX = 0;
	float texY = 0;
	switch (m_FitMode)
	{
	case FIT_STRETCHXY:
		texX = x*m_Width;
		texY = y*m_Height;
		break;
	}
	float ret = 1.0;
	switch (m_IntPolMode)
	{
	case INTPOL_PIXELS:
		ret = getAlpha((int)texX, (int)texY);
		break;
	case INTPOL_LINEAR:
		int idXL = (int)texX;
		float deltaX = texX - (idXL + 0.5f);
		int idXR = idXL;
		if (deltaX>0 && idXL + 1 < m_Width)idXR = idXL + 1;
		else if (idXL >0)idXR = idXL - 1;

		int idYB = (int)texY;
		float deltaY = texY - (idYB + 0.5f);
		int idYT = idYB;
		if (deltaY>0 && idYB + 1 < m_Height)idYT = idYB + 1;
		else if (idYB>0)idYT = idYB - 1;

		float top = getAlpha(idXL, idYT) * (1.f - abs(deltaX)) + getAlpha(idXR, idYT) * (abs(deltaX));
		float bot = getAlpha(idXL, idYB) * (1.f - abs(deltaX)) + getAlpha(idXR, idYB) * (abs(deltaX));
		ret = bot*(1.f - abs(deltaY)) + top*abs(deltaY);
		break;
	}
	return ret;
}

size_t Texture::getWidth(){
	return m_Width;
}
size_t Texture::getHeight(){
	return m_Height;
}
string Texture::getName(){
	return m_Name;
}

float* Texture::GetData()
{
	return m_Pixels;
}
unsigned int Texture::GetLength()
{
	return m_Length;
}