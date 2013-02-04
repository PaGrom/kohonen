#ifndef _Maintainer_hpp
#define _Maintainer_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

class Maintainer {
private:
	string name;
	vector<string> commits;
	vector<string> files;
public:
	Maintainer(string my_name);
	~Maintainer();

	void add_commit(string commit);
	void pop_commit();

	void add_file(string file);
	void pop_file();
};

#endif