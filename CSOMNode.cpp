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
		//srand(time(NULL));
		m_weights[i] = rand() % 50;
		//printf("wei = %f\n", m_weights[i]);
	}
	
};


//+------------------------------------------------------------------+
//| Функция возвращает значение веса weight_index                    |
//+------------------------------------------------------------------+
double CSOMNode::GetWeight(int weight_index) {
	if(weight_index>=0 && weight_index<m_dimension) {
		//return(m_weights.at(weight_index));
		return m_weights[weight_index];
	}
	else
		return 0;
}
//+------------------------------------------------------------------+
//| Функция возвращает координаты узла                               |
//+------------------------------------------------------------------+
void CSOMNode::GetCoordinates(int &x1,int &y1,int &x2,int &y2) {
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
void CSOMNode::AdjustWeights(vector<double> *vec,double learning_rate,double influence) {
	for(int i=0; i<m_dimension; i++) {
		//m_weights.at(i)=m_weights.at(i)+learning_rate*influence*(vec->at(i)-m_weights.at(i));
		//printf("old = %f\n", m_weights[i]);
		//printf("vec-wei = %f\n", learning_rate);
		m_weights[i] += learning_rate*influence*(vec->at(i) - m_weights[i]);
		//printf("new = %f\n", m_weights[i]);
	}
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CSOMNode::SetDimension(int dimension) {
	m_dimension=dimension;
}