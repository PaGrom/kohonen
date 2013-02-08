#include "CSOM.hpp"

int main(int argc, char const *argv[])
{
	if (argc < 2) { 
		printf("Usage: %s /path/to/git-folder\n", argv[0]);
		exit(1); 
	}

	CSOM som;
	som.Load(argv[1]);
	som.InitParameters(10000,CellsX,CellsY,ImageXSize,ImageYSize);
	som.Train();
	som.Render();
	return 0;
}
