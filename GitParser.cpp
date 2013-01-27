#include "GitParser.hpp"

GitParser::GitParser() {

}

GitParser::~GitParser() {
	
}

vector<string> GitParser::split_string(string source, char split_char) {    
	vector<string> array;

	istringstream is(source);
	string s;
	while (getline(is, s, split_char))
		array.push_back(s);

	for (vector<string>::iterator i = array.begin(); i < array.end(); ++i)
		cout << *i << endl;

	return array;
}

void GitParser::create_commit_file() {
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

	GitParser git;

	git.split_string(source,' ');

	git.create_commit_file();

	return 0;
}