#ifndef _GitParser_hpp
#define _GitParser_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "Maintainer.hpp"

using namespace std;

#define FILE_NAME "cmd_output"
#define SOURCE_FILE "source_file"
#define GIT_PATH "~/Dropbox/Work/realclear/linux/"
#define GIT_LOG "--pretty=tformat:'%cE %H'"
#define GIT_SHOW "--raw --name-only --pretty=tformat:''"
#define NUM_OF_COMMITS 1000

class GitParser {
private:
	string parameters[3];
	map<string, Maintainer*> maintainers;
public:
	GitParser();
	~GitParser();

	vector<string> split_string(string source, char split_char);
	FILE* create_commit_file();
	FILE* create_source_file(string commit);
	string find_path(FILE* source_file);
	void read_file(FILE* pFile);
	
};

#endif