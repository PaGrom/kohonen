#include "Maintainer.hpp"

Maintainer::Maintainer() {
	
}

Maintainer::Maintainer(string my_name) {
	name = my_name;
}

Maintainer::~Maintainer() {
	delete &commits;
	delete &files;
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

string Maintainer::pop_file() {
	string back = files.back();
	files.pop_back();
	return back;
}

int Maintainer::files_size() {
	return files.size();
}