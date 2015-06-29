#include "Bitmap.hpp"


//----------------------------
//Constructor and Destructor
//----------------------------
Bitmap::Bitmap(int width, int height)
{
	m_Width = width;
	m_Height = height;
	m_Pixels = new Uint32[m_Width * m_Height];
	memset(m_Pixels, 30, m_Width * m_Height * sizeof(Uint32));
}
Bitmap::Bitmap(Texture tex)
{
	m_Width = tex.getWidth();
	m_Height = tex.getHeight();
	m_Pixels = new Uint32[m_Width * m_Height];
	memset(m_Pixels, 30, m_Width * m_Height * sizeof(Uint32));
	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			SetPixel(i, j, tex.getRGB(i, j));
		}
	}
}
Bitmap::~Bitmap()
{
	delete m_Pixels;
	m_Pixels = nullptr;
}
//----------------------------
//Member Functions
//----------------------------
void Bitmap::SetPixel(int x, int y, colRGB col)//Takes gamma into account
{
	if (y >= 0 && y < m_Height && x >= 0 && x < m_Width)
	{
		int red = (int)(min(powf(col.red, (1.f / 2.2f)), 1.f) * 255) << 16;
		int green = (int)(min(powf(col.green, (1.f / 2.2f)), 1.f) * 255) << 8;
		int blue = (int)(min(powf(col.blue, (1.f / 2.2f)), 1.f) * 255);
		Uint32 uint = Uint32(red + green + blue);
		m_Pixels[y * m_Width + x] = uint;
	}
}
Uint32* Bitmap::GetData()
{
	return m_Pixels;
}
int Bitmap::GetWidth()
{
	return m_Width;
}
int Bitmap::GetHeight()
{
	return m_Height;
}