#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <Windows.h>
#include <Commdlg.h>

#include "../../../LightWhatRenderer/Utility/common.hpp"
#include "../../../LightWhatRenderer/Utility/Texture.hpp"

using namespace std;
class ImageExporter
{
public:
	ImageExporter();
	~ImageExporter();

	void saveBMP(const char *filename, int w, int h, int dpi, Texture data);
};