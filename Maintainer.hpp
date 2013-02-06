#ifndef _Maintainer_hpp
#define _Maintainer_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Maintainer {
private:
	string name;
	int sum;
	vector<string> commits;
	vector<string> files;
	vector<string> parameters;
	map<string, int> num_of_parameters;
public:
	Maintainer();
	Maintainer(string my_name);
	~Maintainer();

	string get_name();

	void add_commit(string commit);
	string pop_commit();
	int commits_size();

	void add_file(string file);
	void add_file(vector<string> paths);
	string pop_file();
	int files_size();

	void calculate(vector<string> params);
	vector<int> get_nums();
	void calc_sum();
	int get_sum();
};

#endif
