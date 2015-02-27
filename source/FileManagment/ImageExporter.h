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

#include "../Rendering/common.h"

using namespace std;
class ImageExporter
{
public:
	ImageExporter();
	~ImageExporter();

	void saveBMP(const char *filename, int w, int h, int dpi, vector<vector<colRGB> > data);
};

