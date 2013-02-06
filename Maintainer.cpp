#include "Maintainer.hpp"

Maintainer::Maintainer() {
	
}

Maintainer::Maintainer(string my_name) {
	name = my_name;
	sum = 0;
}

Maintainer::~Maintainer() {
	delete &commits;
	delete &files;
	delete &num_of_parameters;
}

string Maintainer::get_name() {
	return name;
}

void Maintainer::add_commit(string commit) {
	commits.push_back(commit);
}

string Maintainer::pop_commit() {
	string back = commits.back();
	commits.pop_back();
	return back;
}

int Maintainer::commits_size() {
	return commits.size();
}

void Maintainer::add_file(string file) {
	files.push_back(file);
}

void Maintainer::add_file(vector<string> paths) {
	while (paths.size()) {
		files.push_back(paths.back());
		paths.pop_back();
	}
}

string Maintainer::pop_file() {
	string back = files.back();
	files.pop_back();
	return back;
}

int Maintainer::files_size() {
	return files.size();
}

void Maintainer::calculate(vector<string> params) {
	parameters = params;
	for (vector<string>::iterator i = parameters.begin(); i < parameters.end(); ++i)
		num_of_parameters[(*i)] = 0;

	for (vector<string>::iterator i = parameters.begin(); i < parameters.end(); ++i)
		for (vector<string>::iterator it = files.begin(); it < files.end(); ++it)
			if ((*it).find((*i)) != -1)
				num_of_parameters[(*i)] += 1;

	calc_sum();
}

vector<int> Maintainer::get_nums() {
	vector<int> nums;
	for (vector<string>::iterator i = parameters.begin(); i < parameters.end(); ++i)
		nums.push_back(num_of_parameters[(*i)]);
	return nums;
}

void Maintainer::calc_sum() {
	for (map<string, int>::iterator i = num_of_parameters.begin(); i != num_of_parameters.end(); ++i)
		sum += i->second;
}

int Maintainer::get_sum() {
	return sum;
}