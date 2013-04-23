#ifndef _CSOMNode_hpp
#define _CSOMNode_hpp

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <sstream>
#include <cmath>
#include <Magick++.h>

using namespace std;
using namespace Magick;

class CSOMNode
  {
protected:
   int               m_x1;
   int               m_y1;
   int               m_x2;
   int               m_y2;
   int               m_dimension;

   double            m_x;
   double            m_y;
   //vector<double> m_weights;
   double m_weights[10];
public:
   //--- конструктор класса
                     CSOMNode(int dimension);
   //--- деструктор класса
                    ~CSOMNode();
   //--- функция инициализации узла сети
   void              InitNode(int x1,int y1,int x2,int y2, int max);
   //--- функции возвращают координаты центра узла
   double            X()  const { return(m_x);}
   double            Y()  const { return(m_y);}
   //--- функция возвращает координаты узла
   void              GetCoordinates(int &x1,int &y1,int &x2,int &y2);
   //--- функция возвращает значение веса weight_index
   double            GetWeight(int weight_index);
   //--- функция расчета квадрата расстояния до заданного вектора
   double            CalculateDistance(vector<double> *vec);
   //--- функция корректировки весов узла
   void              AdjustWeights(vector<double> *vec,double learning_rate,double influence);
   //--- функция установки размерности узла
   void              SetDimension(int dimension);

  };

#endif
