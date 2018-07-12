// Input/Output
// Manages reading the input files and writing output files.

#include "IO.h"
#include "Environment.h"

#include <iostream>
#include <string.h>

extern Environment env;

IO::Files::Files() {
	total_files = 0;

	defaultfile = "resources/defaults.ctrl"; // where to find default settings
	optionsfile = "resources/options.ctrl"; // where to find optional control settings

	add_file("default", defaultfile, IOtype::INPUT);
	add_file("options", optionsfile, IOtype::INPUT);
}

void IO::Files::add_file(std::string name, std::string path, IOtype t) {
	file_to_index[name] = total_files;

	//if(t == IOtype::INPUT) {
	//	*f = {file_name, t};
	//} else {
	//	*f = {file_name, t};
	//}

	std::string file_name = filename_from_path(path);
	file_values[total_files] = {path, file_name, t};	
	
	std::ifstream file_stream(path);
	check_stream(file_name, file_stream);

	total_files++;
}

std::ifstream IO::Files::read_file(std::string name) {
	int i = file_to_index[name];
	std::string path = file_values[i].path;

	std::ifstream file_stream(path);
	check_stream(file_values[i].path, file_stream);

	return(file_stream);
}

void IO::Files::print() {
	std::cout << std::endl << "Files:" << std::endl;

	for(std::map<std::string, int>::iterator it = file_to_index.begin(); it != file_to_index.end(); ++it) {
		std::cout << it->first << " : " << file_values[it->second].file_name << " " << file_values[it->second].path << " ";
		if(file_values[it->second].t == IOtype::INPUT) {
			std::cout << "INPUT" << std::endl;
		} else {
			std::cout << "OUTPUT" << std::endl;
		}
	}

	std::cout << std::endl;
}

void IO::Files::close() {
	for(std::map<std::string, int>::iterator it = file_to_index.begin(); it != file_to_index.end(); ++it) {
		if(file_values[it->second].t == IOtype::INPUT) {
			auto new_path = findFullFilePath(file_values[it->second].file_name);
			copyFile(file_values[it->second].path, new_path);
		}
	}
	print();
}

void IO::Files::check_stream(const std::string &file_name, std::ifstream &s) {
	if (not s.good()) {
		std::cerr << "Cannot read control file: \"" << file_name << "\"" << std::endl;
		exit(EXIT_FAILURE);
	}
}

std::string IO::Files::filename_from_path(std::string path) {
	char * cstr = new char[path.length() + 1];
	strcpy(cstr, path.c_str());
	
	char *pch;
	pch = strtok(cstr, "/");
	char *lastToken;

	while(pch != NULL) {
		lastToken = pch;
		pch = strtok(NULL, "/");
	}

	return(std::string(lastToken));
}

void IO::Files::copyFile(const string &sourcefile, const string &newfile) {
	std::cout << "Copying" << std::endl;
	std::cout << sourcefile << " " << newfile << std::endl;
	std::ifstream source(sourcefile);
	std::ofstream destination(newfile);
	destination << source.rdbuf();
}

inline std::string IO::Files::findFullFilePath(std::string parameter) {
	/*
	 * Prepends the output directory path to the output file names, giving the full path name
	 * for the given file.
	 */

	if (parameter == "") {
		std::cerr << "Cannot prepend output directory to empty parameter" << std::endl;
	}

	parameter = env.get("output_directory") + parameter;
	return parameter;
}
