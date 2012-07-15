CSOM::CSOM() {
	srand(time(NULL));
	m_initial_learning_rate = 0.1;
	m_training_sets_array = new vector<int>; //m_training_sets_array = new vector< vector<int> >;
	m_train_titles = new vector<string>;
	m_dev_emails = new vector<string>;

	m_som_nodes = new vector<CSOMNode*>;
	images = new vector<Image>;
}

CSOM::~CSOM() {
	delete m_training_sets_array;
	delete m_train_titles;
	delete m_som_nodes;
	delete m_dev_emails;
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
		//printf("Par: %s\tm_min_value=%d\tm_max_value=%d\n",m_train_titles->at(j).c_str(), minv, maxv);
	}

	//--- находим максимальное значение параметров для расчета диапазона случайного веса нейрона
	int maximum = m_max_values.at(0);

	for (int i = 1; i < m_max_values.size(); ++i) {
		if (m_max_values.at(i) > maximum) {
			maximum = m_max_values.at(i);
		}
	}

	//--- инициализируем узлы
	for(int i=0; i<m_xcells; i++) {
		for(int j=0; j<m_ycells; j++) {
			CSOMNode *node = new CSOMNode(m_dimension);
			node->InitNode(i*cellwidth,j*cellheight,(i+1)*cellwidth,(j+1)*cellheight, maximum);
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

	for (int i = 0; i < m_dimension + 1; ++i) {
		images->push_back(Image(Geometry(ImageXSize,ImageYSize), Color("black")));
	}

	Blend(0,255,0,0,0,255,0,0);

	Blend(0,1*64-1,98,98,255,98,255,255);
	Blend(1*64,2*64-1,98,255,255,98,255,98);
	Blend(2*64,3*64-1,98,255,98,255,255,98);
	Blend(3*64,4*64-1,255,255,98,255,98,98);
}

void CSOM::Blend(int c1, int c2, int r1, int g1, int b1, int r2, int g2, int b2) {
	int n = (c2-c1);
	for(int i=0; i<=n; i++) {
		if((c1+i+2) < 768) {
			Palette[3*(c1+i)+0] = round(1*(r1*(n-i) + r2*i) * 1.0 / n);
			Palette[3*(c1+i)+1] = round(1*(g1*(n-i) + g2*i) * 1.0 / n);
			Palette[3*(c1+i)+2] = round(1*(b1*(n-i) + b2*i) * 1.0 / n);
		}
	}
}

string CSOM::GetPalColor(int ind) {

	if(ind<=0)
		ind=0;
	int r=Palette[3*(ind)];
	int g=Palette[3*(ind)+1];
	int b=Palette[3*(ind)+2];

	return(ConvertRGBtoHex(r,g,b));
}

void CSOM::Train() {
	double learning_rate = m_initial_learning_rate;
	int iter=0;
	vector<double> datavector(m_dimension);

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

		m_dev_emails->push_back(developer->Attribute("Email"));

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

string CSOM::ConvertRGBtoHex(int num) {
	static string hexDigits = "0123456789ABCDEF";
	string rgb;
	for (int i=(3*2) - 1; i>=0; i--) {
		rgb += hexDigits[((num >> i*4) & 0xF)];
	}
	return rgb;
}

string CSOM::ConvertRGBtoHex(int r, int g, int b) {
	int rgbNum = ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
	 
	return "#" + ConvertRGBtoHex(rgbNum);
}

void CSOM::Render() {

	int ind=0;
	for(int i=0; i<m_xcells; i++) {
		//printf("%2.2f\n", m_som_nodes->at(i)->GetWeight(0));
		for(int j=0; j<m_ycells; j++) {

			for(int k=0; k < m_dimension; k++) {
				//m_som_nodes[ind].GetCoordinates(x1,y1,x2,y2);
				//printf("i = %d j = %d k = %d\n", i, j, k);
				double range = m_max_values[k] - m_min_values[k];
				double avg = 255*(m_som_nodes->at(ind)->GetWeight(k)-m_min_values[k])/range; 
				string col = GetPalColor(avg); 
				RenderCell(k,col,ind,(j%2==0));
			}

			int r = (255*(m_som_nodes->at(ind)->GetWeight(0)-m_min_values[0])/(m_max_values[0]-m_min_values[0]));
			int g = (255*(m_som_nodes->at(ind)->GetWeight(1)-m_min_values[1])/(m_max_values[1]-m_min_values[1]));
			int b = (255*(m_som_nodes->at(ind)->GetWeight(2)-m_min_values[2])/(m_max_values[2]-m_min_values[2]));
			string col=ConvertRGBtoHex(r,g,b);
			RenderCell(3, col, ind, (j%2==0));
			
			ind++;
		}
	}

	for(int k=0; k < m_dimension + 1; k++) {
		if (ShowTitles)
			ShowPattern(&(images->at(k)));

		//images->at(k).display();
		string title;
		if (k < m_dimension)
			title = m_train_titles->at(k) + ".png";
		else
			title = "main.png";
		images->at(k).write(title);
	}
}

void CSOM::RenderCell(int img, string col, int ind, bool cell_even) {

	int x1,y1,x2,y2;
	m_som_nodes->at(ind)->GetCoordinates(x1,y1,x2,y2);
	

	//printf("%2.2f %d %d %d\n", m_som_nodes->at(i)->GetWeight(0),r,m_min_values[0],m_max_values[0]);

	//printf("1: %d %d 2: %d %d\n", x1,y1,x2,y2);

	int x_size=abs(x2-x1);
	int y_size=abs(y2-y1);
	
	if(cell_even) {
		x1=x1+x_size/2;
		x2=x2+x_size/2;
	}

	y1=y1+y_size/4;
	y2=y2+y_size/4;

	images->at(img).fillColor(col);
	if (!ShowBorders)
		images->at(img).strokeColor(col);
	else
		images->at(img).strokeColor("black");

	images->at(img).strokeWidth(0.005);
	images->at(img).strokeLineJoin(RoundJoin);
	images->at(img).strokeLineCap(RoundCap);
	images->at(img).strokeAntiAlias(true);

	list<Coordinate> coords_of_hexagon;

	coords_of_hexagon.push_back(Coordinate(x1,y1+y_size/4));
	coords_of_hexagon.push_back(Coordinate(x1+x_size/2,y1-y_size/4));
	coords_of_hexagon.push_back(Coordinate(x2,y1+y_size/4));
	coords_of_hexagon.push_back(Coordinate(x2,y2-y_size/4));
	coords_of_hexagon.push_back(Coordinate(x1+x_size/2,y2+y_size/4));
	coords_of_hexagon.push_back(Coordinate(x1,y2-y_size/4));
	//coords_of_hexagon.push_back(Coordinate(x1,y1+y_size/4));

	images->at(img).draw(DrawablePolygon(coords_of_hexagon));
}

void CSOM::ShowPattern(Image *image) {

	vector<double> datavector(m_dimension);

	//printf("%d\n", (int)m_train_titles->size());

	for (int i = 0; i < m_total_training_sets; i++) {
		//printf("i = %d\n", i);

			for(int j = 0; j < m_dimension; j++) 
				datavector.at(j) = m_training_sets_array->at(m_dimension*(i)+j);

			int ind = BestMatchingNode(&datavector);

			int r = (255*(m_som_nodes->at(ind)->GetWeight(0)-m_min_values[0])/(m_max_values[0]-m_min_values[0]));
			int g = (255*(m_som_nodes->at(ind)->GetWeight(1)-m_min_values[1])/(m_max_values[1]-m_min_values[1]));
			int b = (255*(m_som_nodes->at(ind)->GetWeight(2)-m_min_values[2])/(m_max_values[2]-m_min_values[2]));

			//
			//image->strokeWidth(0.1);

			//image->fillColor("black");
			//image->draw(DrawableText(m_som_nodes->at(ind)->X() + 1,m_som_nodes->at(ind)->Y() + 1, m_dev_emails->at(i)) );
			image->strokeColor("white");

			image->strokeWidth(0.005);
			image->strokeLineJoin(RoundJoin);
			image->strokeLineCap(RoundCap);
			image->strokeAntiAlias(true);

			image->fillColor("white");
			image->draw(DrawableText(m_som_nodes->at(ind)->X(),m_som_nodes->at(ind)->Y(), m_dev_emails->at(i)) );
	}
}

void CSOM::Print() {
	for (int i = 0; i<m_som_nodes->size(); i++) {
		if (i % CellsX == 0) {
			printf("%2.2f ", m_som_nodes->at(i)->GetWeight(0));
		}

		
	}
}