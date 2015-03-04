#include "TextureLoader.h"


TextureLoader::TextureLoader()
{
}


TextureLoader::~TextureLoader()
{
}

Texture TextureLoader::getTexture(string fileName, bool &lSuccess){
	ILuint imgName;;
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imgName);
	ilBindImage(imgName);
	success = ilLoadImage(fileName.c_str());
	if (success)
	{
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);
		int size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
		
		ILubyte *Data = ilGetData();
		Texture ret(fileName, width, height);
		ret.setInterpolationMode(INTPOL_PIXELS);
		ret.setQuadraticFittingMode(FIT_STRETCHXY);
		ret.hasAlpha = false;

		for (int i = 0; i < size; i+=4)
		{
			double red =   (int)(Data[i + 0])/255.0;
			double green = (int)(Data[i + 1])/255.0;
			double blue =  (int)(Data[i + 2])/255.0;
			double alpha = (int)(Data[i + 3])/255.0;
			int pixIdx = i / 4;
			if (pixIdx < width*height){
				int x = pixIdx%width;
				int y = pixIdx / width;
				ret.setRGB(colRGB(red, green, blue), x, y);
				ret.setAlpha(alpha, x, y);
			}
		}
		ilBindImage(0);
		ilDeleteImage(imgName);
		lSuccess = true;
		return ret;
	}
	else
	{
		error = ilGetError();
		std::cout << "Image load failed - Image library error: " << error << " - " << iluErrorString(error) << std::endl;
		lSuccess = false;
		return Texture(fileName, 0, 0);
	}
}