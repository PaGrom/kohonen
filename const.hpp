#ifndef _const_hpp
#define _const_hpp

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <ctime>
#include <string>
#include <vector>
#include <iterator>
#include <math.h>
#include <cmath>
#include <Magick++.h>

using namespace std;
using namespace Magick;

// параметр, задает число узлов по X
const int CellsX = 100;
// параметр, задает число узлов по Y
const int CellsY = 100;

// параметр, задает ширину картинки
const int ImageXSize = 750;
// параметр, задает высоту картинки
const int ImageYSize = 750;

//параметр, задает показ границ ячеек
const bool ShowBorders = true;

//параметр, задает показ наименований в координатных плоскостях
const bool ShowTitles = true;

const int weight_size = 100;

#endif
