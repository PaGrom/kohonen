#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>

#define FILE_NAME "cmd_output"
#define GIT_PATH "~/Dropbox/Work/realclear/linux/"
#define GIT_LOG "--pretty=tformat:'%cE %H'"
#define NUM_OF_COMMITS 1000

using namespace std;

vector<string> split_string(string source, char split_char) {    
	vector<string> array;

	istringstream is(source);
	string s;
	while (getline(is, s, split_char))
		array.push_back(s);

	for (vector<string>::iterator i = array.begin(); i < array.end(); ++i)
		cout << *i << endl;

	return array;
}

void create_commit_file() {
	char command[200];
	sprintf(command, "cd %s && git log -n %d %s >> %s", GIT_PATH, NUM_OF_COMMITS, GIT_LOG, FILE_NAME);
	printf("Launch:\n");
	printf("%s\n", command);
	system(command);
}

int main(int argc, char const *argv[]) {

	string source = "A sequence of characters stored consecutively "
						 "in memory and capable of being processed as a "
						 "single entity";

	split_string(source,' ');

	create_commit_file();

	return 0;
}