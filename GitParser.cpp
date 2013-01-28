#include "GitParser.hpp"

GitParser::GitParser() {
	parameters[0] = "arm";
	parameters[1] = "x86";
	parameters[2] = "drivers";
}

GitParser::~GitParser() {
	
}

vector<string> GitParser::split_string(string source, char split_char) {    
	vector<string> array;

	istringstream is(source);
	string s;
	while (getline(is, s, split_char))
		array.push_back(s);

	return array;
}

void GitParser::create_commit_file() {
	char command[200];
	sprintf(command, "cd %s && git log -n %d %s >> %s", GIT_PATH, NUM_OF_COMMITS, GIT_LOG, FILE_NAME);
	printf("Launch:\n");
	printf("%s\n", command);
	system(command);

	sprintf(command, "cp %s%s ./%s && rm %s%s", GIT_PATH, FILE_NAME, FILE_NAME, GIT_PATH, FILE_NAME);
	printf("Launch:\n");
	printf("%s\n", command);
	system(command);
}

void GitParser::create_source_file(string commit) {
	char command[200];
	sprintf(command, "cd %s && git show %s %s >> %s", GIT_PATH, commit.c_str(), GIT_SHOW, SOURCE_FILE);
	printf("Launch:\n");
	printf("%s\n", command);
	system(command);

	sprintf(command, "cp %s%s ./%s && rm %s%s", GIT_PATH, SOURCE_FILE, SOURCE_FILE, GIT_PATH, SOURCE_FILE);
	printf("Launch:\n");
	printf("%s\n", command);
	system(command);
}

void GitParser::remove_file(string file_name) {
	char command[200];
	sprintf(command, "rm %s", file_name.c_str());
	system(command);
}

string GitParser::find_parameters(string file_name) {
	string line;
	ifstream is(SOURCE_FILE);

	while(true) {
		getline(is, line);
		if (!is.eof()) {
			if (line.length())
				return line;
		}
		else
			break;
	}
}

void GitParser::read_file() {
	string line;
	ifstream i(FILE_NAME);

	vector< vector<string> > commit_list;

	while(true) {
		getline(i, line);
		if (!i.eof()) {
			printf("%s\n", line.c_str());
			commit_list.push_back(split_string(line, ' '));

			printf("%s\n", commit_list.back().back().c_str());

			create_source_file(commit_list.back().back());
			commit_list.back().back() = find_parameters(commit_list.back().back());
		}
		else
			break;
	}
}

int main(int argc, char const *argv[]) {

	string source = "A sequence of characters stored consecutively "
						 "in memory and capable of being processed as a "
						 "single entity";

	GitParser git;

	git.create_commit_file();

	git.read_file();

	return 0;
}