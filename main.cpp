#include "const.hpp"
#include "CSOMNode.hpp"
#include "CSOM.hpp"

int main(int argc, char const *argv[])
{
	CSOM som;
	som.Load();
	som.InitParameters(10000,CellsX,CellsY,ImageXSize,ImageYSize);
	som.Train();
	som.Render();
	return 0;
}