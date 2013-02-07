#ifndef _CSOM_hpp
#define _CSOM_hpp

#include "const.hpp"
#include "GitParser.hpp"
#include "CSOMNode.hpp"

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
	//--- вектор с имейлами разработчиков
	vector<string> *m_dev_emails;
	//--- кол-во обучающих итераций
	int m_iterations;
	double m_map_radius;
	double m_time_constant;
	//---коэффициент обучения
	double m_initial_learning_rate;
	//---кол-во параметров в паттерне
	int m_dimension;
	//---вектор с картинками
	vector<Image> *images;
	//---палитра для отрисовки компонентных плоскостей
	int Palette[768];

public:
	CSOM();
	~CSOM();

	void InitParameters(int iterations,int xcells,int ycells,int bmpwidth,int bmpheight);
	void Blend(int c1, int c2, int r1, int g1, int b1, int r2, int g2, int b2);
	string GetPalColor(int ind);
	void Train();
	int BestMatchingNode(vector<double> *vec);
	void LoadXML();
	void Load();
	string ConvertRGBtoHex(int num);
	string ConvertRGBtoHex(int r, int g, int b);
	void Render();
	void RenderCell(int img, string col, int ind, bool cell_even);
	void ShowPattern(Image *image);
};

#endif
