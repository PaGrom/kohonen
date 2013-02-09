#include "CSOM.hpp"

int main(int argc, char **argv)
{
	if (argc < 3) { 
		printf("Usage: %s -p /path/to/git-folder -n number-of-last-commits\n", argv[0]);
		exit(1); 
	}

	char *nvalue = NULL;
	char *pvalue = NULL;
	int index;
	int par;

	string path_to_git;
	int num_of_commits;
	opterr = 0;

	while ((par = getopt (argc, argv, "n:p:")) != -1)
		switch (par) {
			case 'n':
				nvalue = optarg;
				num_of_commits = atoi(nvalue);

				if (!num_of_commits)	{
					printf("Error: number-of-last-commits must be a number and greater than zero!\n");
					printf("Usage: %s -p /path/to/git-folder -n number-of-last-commits\n", argv[0]);
					exit(1); 
				}
				break;

			case 'p':
				pvalue = optarg;
				char command[200];
				sprintf(command, "cd %s/.git", pvalue);

				if (system(command)) {
					printf("Error: %s is not path to git repository!\n", pvalue);
					printf("Usage: %s - p /path/to/git-folder -n number-of-last-commits\n", argv[0]);
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

	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);

	if (!path_to_git.size()) { 
		printf("Usage: %s -p /path/to/git-folder -n number-of-last-commits\n", argv[0]);
		exit(1); 
	}

	CSOM som;
	som.Load(path_to_git, num_of_commits);
	som.InitParameters(10000,CellsX,CellsY,ImageXSize,ImageYSize);
	som.Train();
	som.Render();
	return 0;
}
