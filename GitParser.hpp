#ifndef _GitParser_hpp
#define _GitParser_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

#define FILE_NAME "cmd_output"
#define GIT_PATH "~/Dropbox/Work/realclear/linux/"
#define GIT_LOG "--pretty=tformat:'%cE %H'"
#define NUM_OF_COMMITS 1000

class GitParser {

public:
	GitParser();
	~GitParser();

	vector<string> split_string(string source, char split_char);
	void create_commit_file();
};

#endif