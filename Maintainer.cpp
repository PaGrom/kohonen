#include "Maintainer.hpp"

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

void Maintainer::add_file(string file) {
	files.push_back(file);
}

void Maintainer::pop_file() {
	files.pop_back();
}