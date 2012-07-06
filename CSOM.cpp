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