#include "GitParser.hpp"

GitParser::GitParser(string path_to_git) {
	git_path = path_to_git;
	git_log = "--pretty=tformat:'%cE %H'";
	git_show = "--raw --name-only --pretty=tformat:''";
	num_of_commits = 1000;
	const char *vinit[] = {"arm", "x86", "drivers"};
	parameters = vector<string>(vinit, vinit + sizeof(vinit)/sizeof(char*));
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
	sprintf(command, "cd %s && git log -n %d %s", git_path.c_str(), num_of_commits, git_log.c_str());
	return popen(command, "r");
}

FILE* GitParser::create_source_file(string commit) {
	char command[200];
	sprintf(command, "cd %s && git show %s %s", git_path.c_str(), commit.c_str(), git_show.c_str());
	return popen(command, "r");
}

vector<string> GitParser::find_path(FILE* source_file) {
	if (source_file == NULL) {
		printf("Error opening soure_file\n");
		exit(EXIT_FAILURE);
	}

	char line[200];
	vector<string> paths;

	while (!feof(source_file)) {
		if (fgets(line, 200, source_file) != NULL ) {
			string str = (char*) line;
			str.erase(remove(str.begin(), str.end(), '\n'), str.end());

			if (str.length())
				paths.push_back(str);
		}
		else
			break;
	}

	return paths;
}

void GitParser::read_file(FILE* pFile) {
	if (pFile == NULL) {
		printf("Error opening file\n");
		exit(EXIT_FAILURE);
	}

	char line[200];

	while (!feof(pFile))
		if (fgets(line, 200, pFile) != NULL ) {
			vector<string> vec = split_string((char*)line, ' ');

			if (!maintainers.count(vec.at(0))) { // Add new Maintainer if no item with current name
				Maintainer* mnt = new Maintainer(vec.at(0));
				maintainers[vec.at(0)] = mnt;
			}

			maintainers[vec.at(0)]->add_commit(vec.at(1));
		}

	fclose(pFile);

	for (map<string, Maintainer*>::iterator it=maintainers.begin(); it!=maintainers.end(); ++it)
		while (it->second->commits_size()) {
			FILE* source_file = create_source_file(it->second->pop_commit());
			it->second->add_file(find_path(source_file));
			fclose(source_file);
		}

	for (map<string, Maintainer*>::iterator it=maintainers.begin(); it!=maintainers.end(); ++it)
		it->second->calculate(parameters);
}

vector<string> GitParser::get_parameters() {
	return parameters;
}

vector<string> GitParser::get_emails() {
	vector<string> emails;

	for (map<string, Maintainer*>::iterator it=maintainers.begin(); it!=maintainers.end(); ++it) {
		if ((*it).second->get_sum() == 0) // If maintainer doesn't have commits with our parametres, we skip him
			continue;
		emails.push_back((*it).second->get_name());
	}

	return emails;
}

vector<int> GitParser::calc_training_array() {
	vector<int> training_array;
	for (map<string, Maintainer*>::iterator it=maintainers.begin(); it!=maintainers.end(); ++it) {
		if ((*it).second->get_sum() == 0) // If maintainer doesn't have commits with our parametres, we skip him
			continue;
		vector<int> nums = (*it).second->get_nums();
		for (vector<int>::iterator i = nums.begin(); i < nums.end(); ++i)
			training_array.push_back(*i);
	}

	return training_array;
}
