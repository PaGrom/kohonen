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
	string str;
	while (getline(is, str, split_char)) {
		str.erase(remove(str.begin(), str.end(), '\n'), str.end()); // remove '\n' from strings
		array.push_back(str);
	}

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

string GitParser::find_path(FILE* source_file) {
	if (source_file == NULL) {
		printf("Error opening soure_file\n");
		exit(EXIT_FAILURE);
	}

	char line[200];

	while (!feof(source_file)) {
		if (fgets(line, 200, source_file) != NULL ) {
			string str = (char*) line;
			str.erase(remove(str.begin(), str.end(), '\n'), str.end());

			if (str.length())
				return str;
		}
		else
			break;
	}
}

void GitParser::read_file(FILE* pFile) {
	if (pFile == NULL) {
		printf("Error opening file\n");
		exit(EXIT_FAILURE);
	}

	vector< vector<string> > commit_list;
	char line[200];

	while (!feof(pFile))
		if (fgets(line, 200, pFile) != NULL ) {
			fputs(line, stdout);
			commit_list.push_back(split_string((char*)line, ' '));
			FILE* source_file = create_source_file(commit_list.back().back());
			commit_list.back().back() = find_path(source_file); // change commit number to path
			fclose(source_file);
		}
	fclose(pFile);
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