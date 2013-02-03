#ifndef _GitParser_hpp
#define _GitParser_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

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

public:
	GitParser();
	~GitParser();

	vector<string> split_string(string source, char split_char);
	FILE* create_commit_file();
	FILE* create_source_file(string commit);
	void remove_file(string file_name);
	string find_parameters(string file_name);
	void read_file();
	
};

#endif