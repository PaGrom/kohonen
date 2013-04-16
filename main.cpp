#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "CSOM.hpp"

int main(int argc, char **argv)
{
	char usage[200];

	char *nvalue = NULL;
	char *pvalue = NULL;
	int index;
	int par;
	int option_index;

	string path_to_git;
	int num_of_commits;
	opterr = 0;

	const char* short_options = "hn:p:";

	const struct option long_options[] = {
		{"help",no_argument,NULL,'h'},
		{"path",optional_argument,NULL,'p'},
		{NULL,0,NULL,0}
	};

	while ((par = getopt_long(argc,argv,short_options,
		long_options,&option_index))!=-1)
		switch (par) {
			case 'h':
				printf("Usage: %s [options] [target] ...\n", argv[0]);
				printf("Options:\n");
				printf("   -n,\t\t\tNumber of last commits.\n");
				printf("   -p,\t--path\t\tPath to git folder\n");
				printf("   -h,\t--help\t\tPrint this message and exit.\n");
				break;
			case 'n':
				nvalue = optarg;
				num_of_commits = atoi(nvalue);

				if (!num_of_commits)	{
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

			case '?':
				if (optopt == 'n')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (optopt == 'p')
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
	som.InitParameters(10000,CellsX,CellsY,ImageXSize,ImageYSize);
	som.Train();
	som.Render();
	return 0;
}
