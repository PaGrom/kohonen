#include "GitParser.hpp"

GitParser::GitParser(string path_to_git, int number, vector<string> pars, bool org) {
	git_path = path_to_git;
	git_log = "--pretty=tformat:'%cE %H'";
	git_show = "--raw --name-only --pretty=tformat:''";
	num_of_commits = number;
	parameters = pars;
	organisations = org;
}

GitParser::~GitParser() {

}

FILE* GitParser::create_commit_file() {
	ostringstream command;
	command << "cd " << git_path << " && git log -n " << num_of_commits << " " << git_log << "\n";
	return popen(command.str().c_str(), "r");
}

FILE* GitParser::create_source_file(string commit) {
	ostringstream command;
	command << "cd " << git_path << " && git show " << commit << " " << git_show << "\n";
	return popen(command.str().c_str(), "r");
}

vector<string> GitParser::find_path(FILE *source_file) {
	if (source_file == NULL) {
		cout << "Error opening soure_file" << endl;;
		exit(EXIT_FAILURE);
	}

	char line[200];
	vector<string> paths;

	while (!feof(source_file)) {
		if (fgets(line, 200, source_file) != NULL ) {
			string str = (char*)line;
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
		cout << "Error opening file" << endl;;
		exit(EXIT_FAILURE);
	}

	char line[200];

	while (!feof(pFile))
		if (fgets(line, 200, pFile) != NULL ) {
			vector<string> vec = split((char*)line, ' ');
			
			string name;
			if (organisations)
				name = split(vec.at(0), '@').at(1);
			else
				name = vec.at(0);

			if (!maintainers.count(name)) { // Add new Maintainer if no item with current name
				Maintainer* mnt = new Maintainer(name);
				maintainers[name] = mnt;
			}

			maintainers[name]->add_commit(vec.at(1));
		}

	fclose(pFile);

	for (map<string, Maintainer*>::iterator it = maintainers.begin(); it != maintainers.end(); ++it)
		while (it->second->commits_size()) {
			string str = it->second->pop_commit();
			str.erase(remove(str.begin(), str.end(), '\n'), str.end()); // Удаляем знак новой строки
			FILE *source_file = create_source_file(str);
			it->second->add_files(find_path(source_file));
			fclose(source_file);
		}

	for (map<string, Maintainer*>::iterator it = maintainers.begin(); it != maintainers.end(); ++it)
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
	for (map<string, Maintainer*>::iterator it = maintainers.begin(); it != maintainers.end(); ++it) {
		if ((*it).second->get_sum() == 0) // If maintainer doesn't have commits with our parametres, we skip him
			continue;
		vector<int> nums = (*it).second->get_nums();
		for (vector<int>::iterator i = nums.begin(); i < nums.end(); ++i)
			training_array.push_back(*i);
	}

	return training_array;
}
