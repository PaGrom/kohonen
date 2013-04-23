#ifndef _Maintainer_hpp
#define _Maintainer_hpp

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

class Maintainer {
private:
	//--- Адрес электронной почты разработчика
	string name;
	//--- Общее кол-во измененных файлов, соответствующих заданным параметрам
	int sum;
	//--- Список хеш-кодов коммитов
	vector<string> commits;
	//--- Список измененных файлов
	vector<string> files;
	//--- Список параметров
	vector<string> parameters;
	//--- Имя параметра - кол-во измененных файлов, соответствующих этому параметру
	map<string, int> num_of_parameters;

public:
	Maintainer();
	Maintainer(string my_name);
	~Maintainer();

	//--- Метод возвращает имя
	string get_name();

	//--- Метод добавляет хеш-код коммита в конец списка commits
	void add_commit(string commit);
	//--- Метод удаляет последний коммит из списка
	string pop_commit();
	//--- Метод возвращает кол-во коммитов
	int commits_size();

	//--- Метод добавляет название измененного файла в конец списка files
	void add_file(string file);
	//--- Метод добавляет сразу несколько названий измененных файлов в конец списка files
	void add_files(vector<string> paths);
	//--- Метод удаляет последнее название файла из списка files
	string pop_file();
	//--- Метод возвращает кол-во изменных файлов
	int files_size();

	//--- Метод считает кол-во изменений для каждого параметра
	void calculate(vector<string> params);
	//--- Метод возвращает кол-во изменений для каждого параметра
	vector<int> get_nums();
	//--- Метод считает сумму всех изменений
	void calc_sum();
	//--- Метод возвращает сумму всех изменений
	int get_sum();
};

#endif
