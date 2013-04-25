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

vector<string> split(string s, char sym) {

	stringstream sstr(s);
	string segment;
	vector<string> seglist;

	while(getline(sstr, segment, sym))
		seglist.push_back(segment);

	return seglist;
}

bool check_parameters() {
	int i = 0; // счетсик ошибок
	// проверка количества узлов сети
	if (CellsX <= 0 || CellsY <= 0) {
		printf("Error: wrong number of cells\n");
		i++;
	}

	// проверка размера изображения
	if (ImageXSize <= 0 || ImageXSize > 1920 ||
		ImageYSize <= 0 || ImageYSize > 1080) {
		printf("Error: wrong image size\n");
		i++;
	}

	// проверка количества последних коммитов
	if (num_of_commits <= 0) {
		printf("Error: wrong value of num_of_commits\n");
		i++;
	}

	// проверка git-репозитория
	char command[200];
	sprintf(command, "cd %s/.git", path_to_git.c_str());

	if (system(command)) {
		printf("Error: %s is not path to git repository!\n", path_to_git.c_str());
		i++;
	}

	if (i)
		return false;
	else
		return true;
}

int main(int argc, char **argv) {

	char *nvalue = NULL;
	char *pvalue = NULL;
	char *cvalue = NULL;
	char *ivalue = NULL;

	int index;
	int par;
	int option_index;

	opterr = 0;

	const char* short_options = "hn:p:btc:i:";

	const struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"path", optional_argument, NULL, 'p'},
		{"show_borders", no_argument, NULL, 'b'},
		{"show_titles", no_argument, NULL, 't'},
		{"cells_xy", required_argument, NULL, 'c'},
		{"image_xy", required_argument, NULL, 'i'},
		{NULL,0,NULL,0}
	};

	vector<string> v;

	while ((par = getopt_long(argc,argv,short_options,
		long_options,&option_index))!=-1)
		switch (par) {
			case 'h':
				printf("Usage: %s [options] [target] ...\n", argv[0]);
				printf("Options:\n");
				printf("   -n\t<num>\t\t\tNumber of last commits.\n");
				printf("   -p,\t--path <path>\t\tPath to git folder.\n");
				printf("   -b,\t--show_borders\t\tShow borders of hexagons.\n");
				printf("   -t,\t--show_titles\t\tShow titles on images.\n");
				printf("   -i,\t--image_xy <x>x<y>\tSetup size of images.\n");
				printf("   -c,\t--cells_xy <x>x<y>\tSetup number of cells.\n");
				printf("   -h,\t--help\t\t\tPrint this message and exit.\n");
				exit(0);
				break;

			case 'n':
				nvalue = optarg;
				num_of_commits = atoi(nvalue);

				if (!num_of_commits) {
					printf("Error: number-of-last-commits must be a number and greater than zero!\n");
					exit(1); 
				}
				break;

			case 'p':
				pvalue = optarg;

				path_to_git = (char*)pvalue;
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

			case '?':
				if (optopt == 'n')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'p')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'i')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;

			default:
				abort();
		}

	for (index = optind; option_index < argc; option_index++)
		printf ("Non-option argument %s\n", argv[index]);

	// проверка параметров
	if (!check_parameters())
		exit(0);

	CSOM som;
	som.Load(path_to_git, num_of_commits);
	som.InitParameters(10000, borders, titles, CellsX, CellsY, ImageXSize, ImageYSize);
	som.Train();
	som.Render();
	return 0;
}
