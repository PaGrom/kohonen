#ifndef _GitParser_hpp
#define _GitParser_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <sstream>
#include <cmath>
#include <Magick++.h>

#include "funcs.hpp"
#include "Maintainer.hpp"

using namespace std;
using namespace Magick;

class GitParser {
private:
	//--- Адрес к git репозиторию
	string git_path;
	//--- Набор ключей командной строки для команды git log
	string git_log;
	//--- Набор ключей командной строки для команды git show
	string git_show;
	//--- Количество проссматриваемых последних коммитов
	int num_of_commits;
	//--- Список названий обучающих паттернов
	vector<string> parameters;
	//--- Информация о разработчиках
	map<string, Maintainer*> maintainers;

public:
	GitParser(string path_to_git, int number, vector<string> pars);
	~GitParser();

	//--- Метод выполняет команду git log и возвращает файл с выводом
	FILE* create_commit_file();
	//--- Метод выполняет команду git show и возвращает файл с выводом
	FILE* create_source_file(string commit);
	//--- Метод читает файл, возвращенный методом create_source_file()
	//--- и возвращает список изменненых коммитом файлов
	vector<string> find_path(FILE* source_file);
	//--- Метод читает файл, возвращенный методом create_commit_file()
	void read_file(FILE* pFile);
	//--- Метод возвращает список названий паттернов
	vector<string> get_parameters();
	//--- Метод возвращает список адресов электронной почты разработчиков
	vector<string> get_emails();
	//--- Метод возвращает список обучающих паттернов
	vector<int> calc_training_array();
};

#endif
