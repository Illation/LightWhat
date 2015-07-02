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
		int red = (int)(min(col.red, 1.f) * 255) << 16;
		int green = (int)(min(col.green, 1.f) * 255) << 8;
		int blue = (int)(min(col.blue, 1.f) * 255);
		Uint32 uint = Uint32(red + green + blue);
		m_Pixels[y * m_Width + x] = uint;
	}
}
void Bitmap::SetPixel(int x, int y, colRGB col, float gamma)//Takes gamma into account
{
	if (y >= 0 && y < m_Height && x >= 0 && x < m_Width)
	{
		int red = (int)(min(powf(col.red, gamma), 1.f) * 255) << 16;
		int green = (int)(min(powf(col.green, gamma), 1.f) * 255) << 8;
		int blue = (int)(min(powf(col.blue, gamma), 1.f) * 255);
		Uint32 uint = Uint32(red + green + blue);
		m_Pixels[y * m_Width + x] = uint;
	}
}
void Bitmap::SetFromTexture(Texture* texPtr)
{
	if ((m_Width == texPtr->getWidth()) && (m_Height == texPtr->getHeight()))
	{
		float* pixelData = texPtr->GetData();
		int j;
		for (int i = j = 0; i < texPtr->GetLength(); i += 4)
		{
			m_Pixels[j] = Uint32(((int)(pixelData[i]*255) << 16) 
				+((int)(pixelData[i + 1]*255)<<8)
				+(int)(pixelData[i + 2]*255));
			j++;
		}
	}
	else
	{
		for (int i = 0; i < m_Width; i++)
		{
			for (int j = 0; j < m_Height; j++)
			{
				SetPixel(i, j, texPtr->getRGB(((float)i / (float)m_Width), ((float)j / (float)m_Height)));
			}
		}
	}
}
void Bitmap::SetFromTexture(Texture* texPtr, float gamma)
{
	if ((m_Width == texPtr->getWidth()) && (m_Height == texPtr->getHeight()))
	{
		float* pixelData = texPtr->GetData();
		int j;
		for (int i = j = 0; i < texPtr->GetLength(); i += 4)
		{
			m_Pixels[j] = Uint32(((int)(min(powf(pixelData[i], gamma), 1.f) * 255) << 16)
				+ ((int)(min(powf(pixelData[i+1], gamma), 1.f) * 255) << 8)
				+ (int)(min(powf(pixelData[i+2], gamma), 1.f) * 255));
			j++;
		}
	}
	else
	{
		for (int i = 0; i < m_Width; i++)
		{
			for (int j = 0; j < m_Height; j++)
			{
				SetPixel(i, j, texPtr->getRGB(((float)i / (float)m_Width), ((float)j / (float)m_Height)), gamma);
			}
		}
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