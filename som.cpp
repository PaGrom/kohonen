#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <vector>
#include <iterator>
#include <math.h>
#include "tinyxml/tinyxml.h"

using namespace std;

// параметр, задает число узлов по X
int CellsX=100;
// параметр, задает число узлов по Y
int CellsY=100;

// параметр, задает ширину картинки
int ImageXSize=250;
// параметр, задает высоту картинки
int ImageYSize=250;

const int weight_size = 100;

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
   double m_weights[weight_size];
public:
   //--- конструктор класса
                     CSOMNode(int dimension);
   //--- деструктор класса
                    ~CSOMNode();
   //--- функция инициализации узла сети
   void              InitNode(int x1,int y1,int x2,int y2);
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
//+------------------------------------------------------------------+
//| Конструктор класса                                               |
//+------------------------------------------------------------------+
CSOMNode::CSOMNode(int dimension)
  {
   m_x1=0;
   m_y1=0;
   m_x2=0;
   m_y2=0;
   m_x=0.0;
   m_y=0.0;

   m_dimension = dimension;

   //m_weights = new vector<double>;
  };

//+------------------------------------------------------------------+
//| Деструктор класса                                                |
//+------------------------------------------------------------------+
CSOMNode::~CSOMNode() {
	//delete m_weights;
};

//+------------------------------------------------------------------+
//| Инициализация параметров узла                                    |
//+------------------------------------------------------------------+
void CSOMNode::InitNode(int x1,int y1,int x2,int y2) {
	srand(time(NULL));
	//--- устанавливаем координаты узла
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;

	//--- расчет координат центра узла
	m_x = x1 + abs((x2-x1))/2;
	m_y = y1 + abs((y2-y1))/2;
	//printf("m_dimension = %d\n", m_dimension);
	//--- инициализация весов случайными значениями;
	for(int i=0; i<m_dimension; i++) {
		//m_weights.push_back(rand() % 255);
		m_weights[i] = rand() % 255;
		//printf("wei = %f\n", m_weights[i]);
	}
	
};


//+------------------------------------------------------------------+
//| Функция возвращает значение веса weight_index                    |
//+------------------------------------------------------------------+
double CSOMNode::GetWeight(int weight_index)
  {
   if(weight_index>=0 && weight_index<m_dimension)
     {
      //return(m_weights.at(weight_index));
     	return m_weights[weight_index];
     }
   else return(0);
  }
//+------------------------------------------------------------------+
//| Функция возвращает координаты узла                               |
//+------------------------------------------------------------------+
void CSOMNode::GetCoordinates(int &x1,int &y1,int &x2,int &y2)
  {
   x1=m_x1;
   y1=m_y1;
   x2=m_x2;
   y2=m_y2;
  }
//+------------------------------------------------------------------+
//| Расчет квадрата расстояния между весами и заданным вектором      |
//+------------------------------------------------------------------+
double CSOMNode::CalculateDistance(vector<double> *vec) {
	//printf("127\n");
	double dist_sqr = 0;
	//printf("size = %d\n", m_weights.size());
	//printf("%d\n", m_dimension);
	for(int i=0; i<m_dimension; i++) {
		//printf("vec = %f\n", vec->at(i));
		//printf("wei1 = %f\n", m_weights[i]);
		//dist_sqr += (vec->at(i) - m_weights.at(i)) * (vec->at(i) - m_weights.at(i));
		dist_sqr += (vec->at(i) - m_weights[i]) * (vec->at(i) - m_weights[i]);
	}
	//printf("142\n");
	return dist_sqr;
};
//+------------------------------------------------------------------+
//| Корректировка весов узла в направлении заданного вектора         |
//+------------------------------------------------------------------+
void CSOMNode::AdjustWeights(vector<double> *vec,double learning_rate,double influence)
  {
   for(int i=0; i<m_dimension; i++)
     {
      //m_weights.at(i)=m_weights.at(i)+learning_rate*influence*(vec->at(i)-m_weights.at(i));
     //printf("old = %f\n", m_weights[i]);
     	//printf("vec-wei = %f\n", learning_rate);
      m_weights[i] += learning_rate*influence*(vec->at(i) - m_weights[i]);
     // printf("new = %f\n", m_weights[i]);
     }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CSOMNode::SetDimension(int dimension)
  {
   m_dimension=dimension;
  }



class CSOM
{
	//--- размеры BMP-картинки
	int m_xsize;
	int m_ysize;
	//--- количество узлов
	int m_xcells;
	int m_ycells;
	//--- вектор узлов сети Кохонена
	vector<CSOMNode*> *m_som_nodes;
	//--- вектор с обучающими паттернами
	vector<int> *m_training_sets_array; //vector< vector<int> > *m_training_sets_array;
	vector<int> m_max_values;
	vector<int> m_min_values;
	//--- кол-во обучающих паттернов
	int m_total_training_sets;
	//--- вектор с названиями обучающих паттернов
	vector<string> *m_train_titles;
	//--- кол-во обучающих итераций
	int m_iterations;
	double m_map_radius;
	double m_time_constant;
	//---коэффициент обучения
	double m_initial_learning_rate;

	int m_dimension;

public:
	CSOM();
	~CSOM();

	void InitParameters(int iterations,int xcells,int ycells,int bmpwidth,int bmpheight);
	void Train();
	int BestMatchingNode(vector<double> *vec);
	void LoadXML();
	void Print();
	void Save();
};

CSOM::CSOM() {
	m_initial_learning_rate = 0.1;
	m_training_sets_array = new vector<int>; //m_training_sets_array = new vector< vector<int> >;
	m_train_titles = new vector<string>;

	m_som_nodes = new vector<CSOMNode*>;
}

CSOM::~CSOM() {
	delete m_training_sets_array;
	delete m_train_titles;
	delete m_som_nodes;
}

void CSOM::InitParameters(int iterations,int xcells,int ycells,int bmpwidth,int bmpheight) {

	m_xsize=bmpwidth;
	m_ysize=bmpheight;

	m_xcells=xcells;
	m_ycells=ycells;

	//--- задаем количество итераций   
	m_iterations=iterations;
	int cellwidth=m_xsize/m_xcells;
	int cellheight=m_ysize/m_ycells;

	//--- инициализируем узлы
	for(int i=0; i<m_xcells; i++) {
		for(int j=0; j<m_ycells; j++) {
			CSOMNode *node = new CSOMNode(m_dimension);
			node->InitNode(i*cellwidth,j*cellheight,(i+1)*cellwidth,(j+1)*cellheight);
			m_som_nodes->push_back(node);
		}
	}
	//--- вычисляем начальный радиус окрестности
	if(m_xsize>m_ysize)
		m_map_radius=m_xsize/2.0;
	else
		m_map_radius=m_ysize/2.0;
	//printf("m_map_radius = %f\n",m_map_radius);
	m_time_constant=1.0*m_iterations/log(m_map_radius);
}

void CSOM::Train() {
	double learning_rate = m_initial_learning_rate;
	int iter=0;
	vector<double> datavector(m_dimension);

	//--- находим максимальные и минимальные значения компонент обучающего набора
	for(int j=0; j<m_dimension; j++) {
		int maxv = m_training_sets_array->at(j);
		int minv = m_training_sets_array->at(j);

		for(int i=1; i<m_total_training_sets; i++) {
			int v = m_training_sets_array->at(m_dimension*i+j);
			if (v>maxv)
				maxv=v;
			if(v<minv)
				minv=v;
		}

		m_max_values.push_back(maxv);
		m_min_values.push_back(minv);
		printf("Par: %s\tm_min_value=%d\tm_max_value=%d\n",m_train_titles->at(j).c_str(), minv, maxv);
	}

	int total_nodes = m_som_nodes->size();

	int frame=0;
	do {
		//--- выбираем случайным образом индекс вектора из обучающего множества
		int ind = rand() % m_total_training_sets;
		//printf("ind = %d\n", ind);
		//--- устанавливаем datavector его значениями

		for(int k=0; k<m_dimension; k++) {
			datavector.at(k) = m_training_sets_array->at(m_dimension*(ind)+k);
		}
		//--- находим индекс узла сети, наиболее близкого к datavector
		int winningnode = BestMatchingNode(&datavector);
		//printf("272\n");
		//--- определяем текущий радиус окрестности
		double neighbourhood_radius = m_map_radius*exp(-1.0*iter/m_time_constant);
		//printf("286\n");
		//--- цикл по всем узлам сети
		for(int i=0; i<total_nodes; i++) {
			//--- расчет квадрата расстояния до узла с индексом m_WinningNodeIndex
			double DistToNodeSqr = (m_som_nodes->at(winningnode)->X() - m_som_nodes->at(i)->X()) * (m_som_nodes->at(winningnode)->X() - m_som_nodes->at(i)->X())
			                	 + (m_som_nodes->at(winningnode)->Y() - m_som_nodes->at(i)->Y()) * (m_som_nodes->at(winningnode)->Y() - m_som_nodes->at(i)->Y());
			//--- квадрат радиуса окрестности                              
			double WS = neighbourhood_radius * neighbourhood_radius;
			//--- если узел внутри окрестности, то пересчитываем веса
			if(DistToNodeSqr < WS) {
				//--- расчет степени влияния на узел
				double influence = exp(-DistToNodeSqr/(2 * WS));
				//printf("influence = %f\n", influence);
				//--- корректировка узлов в направлении выбранного обучающего вектора
				m_som_nodes->at(i)->AdjustWeights(&datavector,learning_rate,influence);
			}
	    }
		//--- экспоненциальное (по iter) уменьшение коэффициента обучения
		learning_rate = m_initial_learning_rate * exp(-1.0 * iter/m_iterations);
		//printf("learning_rate = %f\n", m_initial_learning_rate);
		//--- увеличиваем счетчик итераций
		iter++;
	}
	//--- продолжаем цикл до тех пор, пока не будет выполнено заданное число итераций
	while(iter<m_iterations);
}

int CSOM::BestMatchingNode(vector<double> *vec) {
	//printf("300\n");
	int min_ind=0;
	double min_dist = m_som_nodes->at(min_ind)->CalculateDistance(vec);
	//printf("303\n");
	int total_nodes = m_som_nodes->size();
	for(int i=0; i<total_nodes; i++) {
		if(m_som_nodes->at(i)->CalculateDistance(vec)<min_dist) {
			min_dist=m_som_nodes->at(i)->CalculateDistance(vec);
			min_ind=i;
		};
	}
	return min_ind;
}

void CSOM::LoadXML() {

	TiXmlDocument *xml_file = new TiXmlDocument("d_list.xml");

	if(!xml_file->LoadFile()) {
		printf("Проблема с XML-файлом\n");
		return;
	}

	TiXmlElement *root = xml_file->FirstChildElement("GitLinux");

	TiXmlElement *parameters = root->FirstChildElement("Parameter");

	while (parameters != NULL) {
		m_train_titles->push_back(parameters->Attribute("par"));

		parameters = parameters->NextSiblingElement("Parameter");
	}

	TiXmlElement *developer = root->FirstChildElement("Developer");

	while(developer != NULL) {
		//vector<int> v;

		for (vector<string>::iterator it = m_train_titles->begin(); it < m_train_titles->end(); it++) {
			
			m_training_sets_array->push_back(atoi(developer->Attribute(it->c_str())));
		}

		//m_training_sets_array->push_back(v);

		developer = developer->NextSiblingElement("Developer");
	}

	m_dimension = m_train_titles->size();
	m_total_training_sets = m_training_sets_array->size() / m_dimension;

	delete root;
	delete developer;
	delete parameters;
}

void CSOM::Save() {
	FILE * fo;
	fo = fopen("rgb.ppm","wt");
	fprintf(fo, "P3\n#Kohonen\n%d %d\n255\n", CellsX, CellsY); 
	for (int i = 0; i < m_som_nodes->size(); ++i) {
		int r = (int)(255*(m_som_nodes->at(i)->GetWeight(0)-m_min_values[0])/(m_max_values[0]-m_min_values[0]));
		int g = (int)(255*(m_som_nodes->at(i)->GetWeight(1)-m_min_values[1])/(m_max_values[1]-m_min_values[1]));
		int b = (int)(255*(m_som_nodes->at(i)->GetWeight(2)-m_min_values[2])/(m_max_values[2]-m_min_values[2]));
		fprintf( fo, "%d %d %d \n", r, g, b);
	}
	fclose(fo); 
}

void CSOM::Print() {
	for (int i = 0; i<m_som_nodes->size(); i++) {
		if (i % CellsX == 0) {
			printf("\n");
		}

		printf("%2.2f ", m_som_nodes->at(i)->GetWeight(0));
	}
}

int main(int argc, char const *argv[])
{
	CSOM som;
	som.LoadXML();
	som.InitParameters(1000,CellsX,CellsY,ImageXSize,ImageYSize);
	som.Train();
	som.Print();
	som.Save();
	return 0;
}