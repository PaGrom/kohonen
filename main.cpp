#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <Magick++.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "funcs.hpp"
#include "CSOM.hpp"

using namespace std;
using namespace Magick;

//количество итераций обучения
int iterations;

// печатать контур ячеек
bool borders = false;
// печатать подписи
bool titles = false;

// параметр, задает число узлов по X
int CellsX;
// параметр, задает число узлов по Y
int CellsY;

// параметр, задает ширину картинки
int ImageXSize;
// параметр, задает высоту картинки
int ImageYSize;

// параметр задает директорию для сохранения получившихся картинок
string image_dir;

// путь до git-репозитория
string path_to_git;
// количество последних коммитов для обработки
int num_of_commits;
// список названий обучающих паттерно
vector<string> patterns;

// парсинг только по доменам организвций, в не по всему email-адресу
bool organisations = false;

// имя xml-файла для сохранения данных
string save_xml_file;
// имя xml-файла для загрузки данных
string load_xml_file;
// сохранять данные в xml-файл
bool save_xml = false;
// загрузить данные из xml-файла
bool load_xml = false;

bool load_from_config(char* config_file) {

	ifstream file(config_file);

	stringstream is_file;

	if (file) {
		is_file << file.rdbuf();
		file.close();
	}
	else
		return false;

	string line;
	while (getline(is_file, line)) {
		istringstream is_line(line);
		string key;
		if (getline(is_line, key, '=')) {
			string value;
			if (getline(is_line, value)) {
				if (key == "borders") {
					if (value == "true")
						borders = true;
					else if (value == "false")
						borders = false;
					else
						cout << "Config error: wrong value of borders. Must be true or false" << endl;
				}
				else if (key == "titles") {
					if (value == "true")
						titles = true;
					else if (value == "false")
						titles = false;
					else
						cout << "Config error: wrong value of titles. Must be true or false" << endl;
				}
				else if (key == "organisations") {
					if (value == "true")
						titles = true;
					else if (value == "false")
						titles = false;
					else
						cout << "Config error: wrong value of organisations. Must be true or false" << endl;
				}
				else if (key == "cells_xy") {
					vector<string> v = split(value, 'x');
					CellsX = atoi(v.at(0).c_str());
					CellsY = atoi(v.at(1).c_str());
				}
				else if (key == "image_xy") {
					vector<string> v = split(value, 'x');
					ImageXSize = atoi(v.at(0).c_str());
					ImageYSize = atoi(v.at(1).c_str());
				}
				else if (key == "image_dir") {
					image_dir = value;
				}
				else if (key == "path_to_git") {
					path_to_git = value;
				}
				else if (key == "num_of_commits") {
					num_of_commits = atoi(value.c_str());
				}
				else if (key == "iterations") {
					iterations = atoi(value.c_str());
				}
				else if (key == "patterns") {
					patterns = split(value, ',');
				}
				else if (key == "save_xml") {
					save_xml_file = value;
					save_xml = true;
				}
				else if (key == "load_xml") {
					load_xml_file = value;
					load_xml = true;
				}
			}
		}
	}

	return true;
}

void parse_commandline(int argc, char **argv) {
	char *nvalue = NULL;
	char *Pvalue = NULL;
	char *cvalue = NULL;
	char *ivalue = NULL;
	char *Cvalue = NULL;
	char *pvalue = NULL;
	char *Ivalue = NULL;
	char *Dvalue = NULL;
	char *Svalue = NULL;
	char *Lvalue = NULL;

	int index;
	int par;
	int option_index;

	opterr = 0;

	const char* short_options = "C:hn:P:btc:i:D:p:I:S:L:o";

	const struct option long_options[] = {
		{"config", no_argument, NULL, 'C'},
		{"num_of_commits", required_argument, NULL, 'n'},
		{"help", no_argument, NULL, 'h'},
		{"path", optional_argument, NULL, 'P'},
		{"show_borders", no_argument, NULL, 'b'},
		{"show_titles", no_argument, NULL, 't'},
		{"cells_xy", required_argument, NULL, 'c'},
		{"image_xy", required_argument, NULL, 'i'},
		{"image_dir", required_argument, NULL, 'D'},
		{"patterns", required_argument, NULL, 'p'},
		{"iterations", required_argument, NULL, 'I'},
		{"save_xml", required_argument, NULL, 'S'},
		{"load_xml", required_argument, NULL, 'L'},
		{"organisations", no_argument, NULL, 'o'},
		{NULL,0,NULL,0}
	};

	vector<string> v;

	while ((par = getopt_long(argc,argv,short_options,
		long_options,&option_index))!=-1)
		switch (par) {
			case 'h':
				cout << "Usage: " << argv[0] << " [options] [target] ..." << endl;
				cout << "Options:" << endl;
				cout << "   -C,\t--config <file>\t\t\tLoad parameters from config file." << endl;
				cout << "   -I,\t--iterations <num>\t\tNumber of iterations for network train." << endl;
				cout << "   -n,\t--num_of_commits <num>\t\tNumber of last commits." << endl;
				cout << "   -P,\t--path <path>\t\t\tPath to git folder." << endl;
				cout << "   -b,\t--show_borders\t\t\tShow borders of hexagons." << endl;
				cout << "   -t,\t--show_titles\t\t\tShow titles on images." << endl;
				cout << "   -i,\t--image_xy <x>x<y>\t\tSetup size of images. Max of X is 1920. Max of Y is 1080." << endl;
				cout << "   -c,\t--cells_xy <x>x<y>\t\tSetup number of cells." << endl;
				cout << "   -D,\t--image_dir <path>\t\tSetup folder for saving images." << endl;
				cout << "   -p,\t--patterns <pattern_1,...>\tSetup patterns." << endl;
				cout << "   -S,\t--save_xml <file_name>\t\tSave data to xml." << endl;
				cout << "   -L,\t--load_xml <file_name>\t\tLoad data from xml." << endl;
				cout << "   -o,\t--organisations\t\t\tParse organisations domains" << endl;
				cout << "   -h,\t--help\t\t\t\tPrint this message and exit." << endl;
				exit(0);
				break;

			case 'C':
				Cvalue = optarg;
				if (!load_from_config(Cvalue))
					cout << "Error: wrong config file." << endl;
				break;

			case 'I':
				Ivalue = optarg;
				iterations = atoi(Ivalue);
				break;

			case 'n':
				nvalue = optarg;
				num_of_commits = atoi(nvalue);
				break;

			case 'P':
				Pvalue = optarg;
				path_to_git = (char*)Pvalue;
				break;

			case 'b':
				borders = true;
				break;

			case 't':
				titles = true;
				break;

			case 'c':
				cvalue = optarg;
				v = split(cvalue, 'x');
				CellsX = atoi(v.at(0).c_str());
				CellsY = atoi(v.at(1).c_str());
				break;

			case 'i':
				ivalue = optarg;
				v = split(ivalue, 'x');
				ImageXSize = atoi(v.at(0).c_str());
				ImageYSize = atoi(v.at(1).c_str());
				break;

			case 'D':
				Dvalue = optarg;
				image_dir = (char*)Dvalue;
				break;

			case 'p':
				pvalue = optarg;
				patterns = split(pvalue, ',');
				break;

			case 'S':
				Svalue = optarg;
				save_xml_file = (char*)Svalue;
				save_xml = true;
				break;

			case 'L':
				Lvalue = optarg;
				load_xml_file = (char*)Lvalue;
				load_xml = true;
				break;

			case 'o':
				organisations = true;
				break;

			case '?':
				if (optopt == 'C')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'n')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'P')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'i')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'p')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'D')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'S')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'L')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);

			default:
				abort();
		}
}

bool check_parameters() {
	int count = 0; // счетсик ошибок
	// проверка количества узлов сети
	if (CellsX <= 0 || CellsY <= 0) {
		cout << "Error: wrong number of cells!" << endl;
		count++;
	}

	// проверка размера изображения
	if (ImageXSize <= 0 || ImageXSize > 1920 ||
		ImageYSize <= 0 || ImageYSize > 1080) {
		cout << "Error: wrong image size!" << endl;
		count++;
	}

	// проверка количества последних коммитов
	if (num_of_commits <= 0) {
		cout << "Error: wrong value of num_of_commits!" << endl;
		count++;
	}

	// проверка количества итераций обучения сети
	if (iterations <= 0) {
		cout << "Error: wrong value of iterations!" << endl;
		count++;
	}

	// проверка паттернов
	if (patterns.size() != 0) {
		if (patterns.size() != 3)
			cout << "There aren't 3 patterns. Can't render main image in RGB." << endl;
		// проверка паттернов на одинаковые имена
		vector<string> temp_patterns = patterns; // копируем вектор паттернов
		sort(temp_patterns.begin(), temp_patterns.end()); // сортируем вектор по возрастанию
		for (int i = 0; i < temp_patterns.size() - 1; ++i) {
			// ищем два одинаковых паттерна рядом
			if (temp_patterns.at(i) == temp_patterns.at(i + 1)) { 
				cout << "Error: similar patterns!" << endl;
				count++;
				break;
			}
		}
		
		// поиск нулевых паттернов
		for (vector<string>::iterator i = patterns.begin(); i != patterns.end(); ++i)
			if ((*i).size() == 0) {
				cout << "Error: zero pattern!" << endl;
				count++;
				break;
			}
	}
	else {
		cout << "Error: there are no patterns!" << endl;
		count++;
	}

	if (!load_xml) {
		// проверка git-репозитория
		if (path_to_git.size() != 0) {
			ostringstream command;
			command << "cd " << path_to_git << "/.git";

			if (system(command.str().c_str())) {
				cout << "Error: " << path_to_git << " is not path to git repository!" << endl;
				count++;
			}
		}
		else {
			cout << "Error: there is no path_to_git!" << endl;
			count++;
		}
	}

	// проверка существования директории для сохранения картинок
	if (image_dir.size() != 0) {
		ostringstream command;
		command << "cd " << image_dir;

		if (system(command.str().c_str())) {
			cout << "Error: " << image_dir << ": No such directory! I'll create it..." << endl;
			mkdir(image_dir.c_str(), 0755);
		}
	}
	else {
		cout << "Error: there is no image_dir!" << endl;
		count++;
	}

	// проверка на корректность имен xml-файлов
	if (load_xml) {
		if (load_xml_file.compare(load_xml_file.length() - 4, 4, ".xml") != 0) {
			cout << "Error: wrong load_xml name!" << endl;
			count++;
		}
	}
	else {
		if (save_xml)
			if (save_xml_file.compare(save_xml_file.length() - 4, 4, ".xml") != 0) {
				cout << "Error: wrong save_xml name!" << endl;
				count++;
			}
	}

	if (count) {
		cout << "Try to use --help" << endl;
		return false;
	}
	else
		return true;
}

int main(int argc, char **argv) {

	parse_commandline(argc, argv);

	// проверка параметров
	if (check_parameters()) {
		CSOM som;
		if (load_xml) {
			cout << "Loading parameters from " << load_xml_file << "..." << endl;
			if(!som.LoadXML(load_xml_file))	{
				cout << "Wrong " << load_xml_file << "!" << endl;
				return 0;
			}
		}
		else {
			cout << "Loading parameters from " << path_to_git << "..." << endl;
			som.LoadGIT(path_to_git, num_of_commits, patterns, organisations);
			if (save_xml) {
				cout << "Saving data to " << save_xml_file << "..." << endl;
				som.SaveXML(save_xml_file);
			}
		}
		cout << "Initialisation parameters..." << endl;
		som.InitParameters(iterations, borders, titles, CellsX, CellsY, ImageXSize, ImageYSize, image_dir);
		cout << "Training neural network..." << endl;
		som.Train();
		cout << "Rendering images..." << endl;
		som.Render();
		cout << "Done!" << endl;
	}

	return 0;
}
