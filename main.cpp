#include "const.hpp"
#include "CSOMNode.hpp"
#include "CSOM.hpp"

int main(int argc, char const *argv[])
{
	CSOM som;
	som.LoadXML();
	som.InitParameters(7000,CellsX,CellsY,ImageXSize,ImageYSize);
	som.Train();
	//som.Print();
	som.Render();
	return 0;
}