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

void Maintainer::pop_commit() {
	commits.pop_back();
}

int Maintainer::commits_size() {
	return commits.size();
}

void Maintainer::add_file(string file) {
	files.push_back(file);
}

void Maintainer::pop_file() {
	files.pop_back();
}

int Maintainer::files_size() {
	return files.size();
}