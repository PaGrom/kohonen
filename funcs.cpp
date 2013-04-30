#include "funcs.hpp"

vector<string> split(string source, char split_char) {

	stringstream sstr(source);
	string segment;
	vector<string> seglist;

	while(getline(sstr, segment, split_char))
		seglist.push_back(segment);

	return seglist;
}