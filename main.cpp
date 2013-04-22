#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "CSOM.hpp"

vector<string> split(string s, char sym) {

	stringstream sstr(s);
	string segment;
	vector<string> seglist;

	while(getline(sstr, segment, sym))
		seglist.push_back(segment);

	return seglist;
}

int main(int argc, char **argv) {

	char usage[200];

	char *nvalue = NULL;
	char *pvalue = NULL;
	char *cvalue = NULL;
	char *ivalue = NULL;

	int index;
	int par;
	int option_index;

	string path_to_git;
	int num_of_commits;
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

	bool borders = false;
	bool titles = false;
	// параметр, задает число узлов по X
	int CellsX = 0;
	// параметр, задает число узлов по Y
	int CellsY = 0;

	// параметр, задает ширину картинки
	int ImageXSize = 0;
	// параметр, задает высоту картинки
	int ImageYSize = 0;

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
					printf("%s", usage);
					exit(1); 
				}
				break;

			case 'p':
				pvalue = optarg;
				char command[200];
				sprintf(command, "cd %s/.git", pvalue);

				if (system(command)) {
					printf("Error: %s is not path to git repository!\n", pvalue);
					printf("%s", usage);
					exit(1); 
				}

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

	if (!path_to_git.size()) { 
		printf("%s", usage);
		exit(1); 
	}

	CSOM som;
	som.Load(path_to_git, num_of_commits);
	som.InitParameters(10000, borders, titles, CellsX, CellsY, ImageXSize, ImageYSize);
	som.Train();
	som.Render();
	return 0;
}
