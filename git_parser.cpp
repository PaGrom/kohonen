#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

vector<string> split_string(string source)
{    
	vector<string> array;

	istringstream is(source);
	string s;
	while (getline(is, s, ' '))
		array.push_back(s);

	for (vector<string>::iterator i = array.begin(); i < array.end(); ++i)
		cout << *i << endl;

	return array;
}

int main(int argc, char const *argv[])
{
	string source = "A sequence of characters stored consecutively "
						 "in memory and capable of being processed as a "
						 "single entity";

	split_string(source);
	return 0;
}