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

#include "CSOM.hpp"

using namespace std;
using namespace Magick;

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

// путь до git-репозитория
string path_to_git;
// количество последних коммитов для обработки
int num_of_commits;
// список названий обучающих паттерно
vector<string> patterns;

vector<string> split(string s, char sym) {

	stringstream sstr(s);
	string segment;
	vector<string> seglist;

	while(getline(sstr, segment, sym))
		seglist.push_back(segment);

	return seglist;
}

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
				else if (key == "path_to_git") {
					path_to_git = value;
				}
				else if (key == "num_of_commits") {
					num_of_commits = atoi(value.c_str());
				}
				else if (key == "patterns") {
					patterns = split(value, ',');
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

	int index;
	int par;
	int option_index;

	opterr = 0;

	const char* short_options = "C:hn:P:btc:i:p:";

	const struct option long_options[] = {
		{"config", no_argument, NULL, 'C'},
		{"num_of_commits", required_argument, NULL, 'n'},
		{"help", no_argument, NULL, 'h'},
		{"path", optional_argument, NULL, 'P'},
		{"show_borders", no_argument, NULL, 'b'},
		{"show_titles", no_argument, NULL, 't'},
		{"cells_xy", required_argument, NULL, 'c'},
		{"image_xy", required_argument, NULL, 'i'},
		{"patterns", required_argument, NULL, 'p'},
		{NULL,0,NULL,0}
	};

	vector<string> v;

	while ((par = getopt_long(argc,argv,short_options,
		long_options,&option_index))!=-1)
		switch (par) {
			case 'h':
				cout << "Usage: " << argv[0] << "[options] [target] ..." << endl;
				cout << "Options:" << endl;
				cout << "   -C,\t--config <file>\t\t\tLoad parameters from config file." << endl;
				cout << "   -n,\t--num_of_commits <num>\t\tNumber of last commits." << endl;
				cout << "   -P,\t--path <path>\t\t\tPath to git folder." << endl;
				cout << "   -b,\t--show_borders\t\t\tShow borders of hexagons." << endl;
				cout << "   -t,\t--show_titles\t\t\tShow titles on images." << endl;
				cout << "   -i,\t--image_xy <x>x<y>\t\tSetup size of images." << endl;
				cout << "   -c,\t--cells_xy <x>x<y>\t\tSetup number of cells." << endl;
				cout << "   -p,\t--patterns <pat1,pat2,pat3>\tSetup patterns." << endl;
				cout << "   -h,\t--help\t\t\t\tPrint this message and exit." << endl;
				exit(0);
				break;

			case 'C':
				Cvalue = optarg;
				if (!load_from_config(Cvalue))
					cout << "Error: wrong config file." << endl;
				break;

			case 'n':
				nvalue = optarg;
				num_of_commits = atoi(nvalue);

				if (!num_of_commits) {
					cout << "Error: number-of-last-commits must be a number and greater than zero!" << endl;
					exit(1); 
				}
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

			case 'p':
				pvalue = optarg;
				patterns = split(pvalue, ',');
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
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);

			default:
				abort();
		}
}

bool check_parameters() {
	int i = 0; // счетсик ошибок
	// проверка количества узлов сети
	if (CellsX <= 0 || CellsY <= 0) {
		cout << "Error: wrong number of cells" << endl;
		i++;
	}

	// проверка размера изображения
	if (ImageXSize <= 0 || ImageXSize > 1920 ||
		ImageYSize <= 0 || ImageYSize > 1080) {
		cout << "Error: wrong image size" << endl;
		i++;
	}

	// проверка количества последних коммитов
	if (num_of_commits <= 0) {
		cout << "Error: wrong value of num_of_commits" << endl;
		i++;
	}

	// проверка паттернов
	if (patterns.size() == 3) {
		if (patterns.at(0) == patterns.at(1) ||
			patterns.at(0) == patterns.at(2) ||
			patterns.at(1) == patterns.at(2)) {

			cout << "Error: similar patterns" << endl;
			i++;
		}
		if (patterns.at(0).size() == 0 ||
			patterns.at(1).size() == 0 ||
			patterns.at(2).size() == 0) {
			
			cout << "Error: zero pattern" << endl;
			i++;
		}
	}
	else {
		cout << "Error: wrong number of patterns" << endl;
		i++;
	}

	// проверка git-репозитория
	ostringstream command;
	command << "cd " << path_to_git << "/.git";

	if (system(command.str().c_str())) {
		cout << "Error: " << path_to_git << " is not path to git repository!" << endl;
		i++;
	}

	if (i) {
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
		som.Load(path_to_git, num_of_commits, patterns);
		som.InitParameters(10000, borders, titles, CellsX, CellsY, ImageXSize, ImageYSize);
		som.Train();
		som.Render();
	}

	return 0;
}
