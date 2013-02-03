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

FILE* GitParser::create_commit_file() {
	char command[200];
	sprintf(command, "cd %s && git log -n %d %s", GIT_PATH, NUM_OF_COMMITS, GIT_LOG);
	printf("Launch:\n");
	printf("%s\n", command);
	return popen(command, "r");
}

FILE* GitParser::create_source_file(string commit) {
	char command[200];
	sprintf(command, "cd %s && git show %s %s", GIT_PATH, commit.c_str(), GIT_SHOW);
	printf("Launch:\n");
	printf("%s\n", command);
	return popen(command, "r");
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

void GitParser::read_file(FILE* pFile) {

	vector< vector<string> > commit_list;

	char line[200];
	if (pFile == NULL)
		perror("Error opening file");
	else {
		while (!feof(pFile)) {
			if (fgets(line, 200, pFile) != NULL ){
				fputs(line, stdout);
				commit_list.push_back(split_string((char*)line, ' '));
				create_source_file(commit_list.back().back());
				commit_list.back().back() = find_parameters(commit_list.back().back());
			}
		}
		fclose (pFile);
	}
}

int main(int argc, char const *argv[]) {

	string source = "A sequence of characters stored consecutively "
						 "in memory and capable of being processed as a "
						 "single entity";

	GitParser git;

	FILE* pFile = git.create_commit_file();

	git.read_file(pFile);

	return 0;
}