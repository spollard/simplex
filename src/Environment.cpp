#include "Environment.h"

#include <cstdlib> // For atoi()
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <typeinfo> //Just for testing.

#ifdef _WIN32
#include <dir.h> // For mkdir()
#include <ctime> // For time()
//#include <Windows.h> // For CreateDirectory. Can't use because of CopyFile
#else
#include <sys/stat.h>// For making directories in Linux and OS X
#include <sys/times.h> // For time()
#endif

Environment::Environment() {
	/*
	 *  Initialize environment class.
	 *  Sets a few minor defaults states, however the majority of the default states are 
	 *  read from default.ctrl.
	 */

	seed = 0;
	total_options = 0;
}

// Initialize options.
void Environment::ReadOptions(std::ifstream &default_file_stream, std::ifstream &options_file_stream) {
	ReadControlFile(default_file_stream);
	ReadControlFile(options_file_stream);
	ProcessOptions();
	PrintOptions();
}

// Read control files.
void Environment::ReadControlFile(std::ifstream &file_stream) {
	std::string key = "";
	std::string value = "";

	std::string line;

	while(std::getline(file_stream, line)) {
		if(line != "") {
			std::istringstream iss(line);
			iss >> key;
			if(key != "#") {
				iss >> value;
				SetOption(key, value);
			}
		}
	}
}

void Environment::SetOption(string option, string value) {
	if(option_to_index.find(option) == option_to_index.end()) {
		//Option does not already exist in options_to_index.
		option_to_index[option] = total_options;
		option_values[total_options] = value;
		total_options++;
	} else {
		//Option does already exist.
		int i = option_to_index[option];
		option_values[i] = value;
	}
}

// Process options.
void Environment::ProcessOptions() {
	debug = get_int("debug");
	outdir = get("output_directory");

	InitializeRandomNumberGeneratorSeed();
	ConfigureOutputDirectory();

	treeout = findFullFilePath(get("tree_out_file"));
	subsout = findFullFilePath(get("substitutions_out_file"));
	lnlout = findFullFilePath(get("likelihood_out_file"));
	seqsout = findFullFilePath(get("sequences_out_file"));
}

void Environment::InitializeRandomNumberGeneratorSeed() {
    if (option_to_index.find("seed") != option_to_index.end()) {
	    seed = get_int("seed");
    } else {
	    seed = time(0) ; 
    }
    srand(seed);
}

// Print options.
void Environment::PrintOptions() {

	std::cout << std::endl << "Options:" << std::endl;

	for(std::map<std::string, int>::iterator it = option_to_index.begin(); it != option_to_index.end(); ++it) {
		std::cout << it->first << " = " << option_values[it->second] << " " << std::endl;
	}

	std::cout << std::endl;
}

// Retreiving options.
void Environment::check_option_exists(std::string option) {
	if(option_to_index.find(option) == option_to_index.end()) {
		std::cerr << "Error: Option \"" << option << "\" not set." << std::endl;
		exit(EXIT_FAILURE);
	}
}

std::string Environment::get(std::string option) {
	check_option_exists(option);

	int i = option_to_index[option];
	return option_values[i];
}

int Environment::get_int(std::string option) {
	return atoi(get(option).c_str());
}

float Environment::get_float(std::string option) {
	return atof(get(option).c_str());
}

// Configuring the output directory.
void Environment::ConfigureOutputDirectory() {
	/*
	 * Configures the output directory and the file names of the output files.
	 * Creates the output directory and changes the file names of the output file to
	 * include to full path to the output directory.
	 *
	 * For example: "seq.out" -> "/output_dir/seq.out"
	 */
	
	char lastchar = outdir.at(outdir.length() - 1);

	if (debug) std::cout << "The last char in " << outdir << " is " << lastchar << std::endl;

	if (lastchar != '/' && lastchar != '\\') {
		if (debug) std::cout << "last char is not /" << std::endl;
		outdir += '/';
	}

	// For Windows
	// According to http://sourceforge.net/p/predef/wiki/OperatingSystems/
	// TODO: double check this works on windows for directory that already exists
#ifdef _WIN32
	if (mkdir(outdir.c_str())) {
		std::cout << "Could not make output directory " << output_directory << std::endl;
	} else {
		std::cout << "Making directory successful" << std::endl;
	}
#else	//ifdef __linux__ || __APPLE__
	struct stat st;
	if (stat(outdir.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
		if (not debug) {
			std::cout << "output dir " << outdir << " exists. Overwrite? (Y/n)" << std::endl;
			if (getchar() != 'Y') exit(1);
		}
	} else {
		mkdir(outdir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
	/*Read, write, and search, or execute, for the file owner; S_IRWXU is the bitwise inclusive
	 * OR of S_IRUSR, S_IWUSR, and S_IXUSR.
	 * Read, write, and search or execute permission for the file's group. S_IRWXG is the
	 * bitwise inclusive OR of S_IRGRP, S_IWGRP, and S_IXGRP. Read, write, and search or
	 * execute permission for users other than the file owner. S_IRWXO is the bitwise inclusive
	 * OR of S_IROTH, S_IWOTH, and S_IXOTH.*/
#endif
}

inline string Environment::findFullFilePath(std::string parameter) {
	/*
	 * Prepends the output directory path to the output file names, giving the full path name
	 * for the given file.
	 */

	if (parameter == "") std::cerr << "Cannot prepend output directory to empty parameter" << std::endl;
	parameter = outdir + parameter;
	return parameter;
}
