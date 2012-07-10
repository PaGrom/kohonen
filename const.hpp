#ifndef _const_hpp
#define _const_hpp

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <vector>
#include <iterator>
#include <math.h>
#include <Magick++.h>
#include "tinyxml/tinyxml.h"

using namespace std;
using namespace Magick;

// параметр, задает число узлов по X
const int CellsX=100;
// параметр, задает число узлов по Y
const int CellsY=100;

// параметр, задает ширину картинки
const int ImageXSize=850;
// параметр, задает высоту картинки
const int ImageYSize=850;

//параметр, задает показ границ ячеек
bool ShowBorders=false;

const int weight_size = 100;

#endif