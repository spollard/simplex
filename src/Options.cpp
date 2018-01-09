#include "Options.h"

#include <cstdlib> // For atoi()
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <dir.h> // For mkdir()
#include <ctime> // For time()
//#include <Windows.h> // For CreateDirectory. Can't use because of CopyFile
#else
#include <sys/stat.h>// For making directories in Linux and OS X
#include <sys/times.h> // For time()
#endif

Options::Options() {
	/*
	 *  Initialize options class.
	 *  Sets a few minor defaults states, however the majority of the default states are 
	 *  read from default.ctrl.
	 */

	defaultfile = "resources/defaults.ctrl";
	optionsfile = "resources/options.ctrl";

	seed = 0;
}

void Options::PrintOptions() {
	std::cout << std::endl << "Options:" << std::endl;
	std::cout << "Default file: " << defaultfile << std::endl;
	std::cout << "Options file: " << optionsfile << std::endl;
	std::cout << "Output directory: " << outdir << std::endl;
	std::cout << "Tree file: " << treefile << std::endl;
	std::cout << "Sequence file: " << seqfile << std::endl;
	std::cout << "Tree output file: " << treeout << std::endl;
	std::cout << "Sequences output file: " << seqsout << std::endl;
	std::cout << "Substitutions output file: " << subsout << std::endl;
	std::cout << "Log Likelihoods output file: " << lnlout << std::endl;
	std::cout << "Seed: " << seed << std::endl; 
	std::cout << "Debug mode: " << debug << std::endl;
	std::cout << "Constant trees: " << constant_tree << std::endl;
	std::cout << "Number of mixture classes: " << mixture_classes << std::endl;
	std::cout << "Number of generations: " << gens << std::endl;
	std::cout << "Output frequency: " << outfreq << std::endl;
	std::cout << "Maximum segment length: " << max_segment_length << std::endl;
	std::cout << "Tree type: " << tree_type << std::endl;
	std::cout << std::endl;
}

void Options::ReadOptions() {
	ReadControlFile(defaultfile);
	ReadControlFile(optionsfile);
	ProcessOptions();
	PrintOptions();
}

void Options::ProcessOptions() {
	InitializeRandomNumberGeneratorSeed(); // InitRandom(seed);
	ConfigureOutputDirectoryNames();                  // meaning what?

	this->CopyFile(optionsfile, outdir + optionsfile);
	this->CopyFile(defaultfile, outdir + defaultfile);

	if (substitution_model_types.size() > 1) { // Remove substitution model type read from default file
		substitution_model_types.pop();  // this seems clunky
	}  if (mixture_classes < 1) mixture_classes = 1;
}

void Options::ReadControlFile(string control_file) {
	std::cout << std::endl;
	std::cout << "Reading options from " << control_file << std::endl;

	std::ifstream controlfile_stream(control_file.c_str());
	if (not controlfile_stream.good()) {
		std::cerr << "Cannot read control file \"" << control_file << "\"" << std::endl;
		exit(-1);
	}

	std::string key = "";
	std::string value = "";

	std::string line;

	while(std::getline(controlfile_stream, line)) {
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

// there is no check if options are of right type
void Options::SetOption(string option, string value) {
	if (option == "debug") debug = atoi(value.c_str());
	else if (option == "seed") seed = atoi(value.c_str());
	else if (option == "tree_file") treefile = value;
	else if (option == "sequences_file") seqfile = value;
	else if (option == "tree_out_file") treeout = value;
	else if (option == "substitutions_out_file") subsout = value;
	else if (option == "sequences_out_file") seqsout = value;
	else if (option == "likelihood_out_file") lnlout = value;
	else if (option == "output_frequency") outfreq = atoi(value.c_str());
	else if (option == "generations") gens = atoi(value.c_str());
	else if (option == "constant_tree") constant_tree = atoi(value.c_str());
	else if (option == "tree_type") tree_type = atoi(value.c_str());
	else if (option == "max_segment_length") max_segment_length = atof(value.c_str());
	else if (option == "substitution_model_type") substitution_model_types.push(atoi(value.c_str()));
	else if (option == "constant_substitution_model") constant_substitution_models.push(atoi(value.c_str()));
	else if (option == "substitution_model_initialization") substitution_models_initialization_type.push(atoi(value.c_str()));
	else if (option == "substitution_model_initialization_file") substitution_models_initialization_files.push(value);
	else if (option == "mixture_classes") mixture_classes = atoi(value.c_str());
	else if (option == "output_directory") outdir = value;
	else { //STP: Unrecognized option is a non-fatal error so simply print warning
		std::cerr << "Unrecognized option found in control file:" << std::endl;
		std::cerr << "\"" << option << "\" with a value \"" << value << "\"" << std::endl;
	}
}

// Might remove this function because it is never called
// but it should be
void Options::WriteOptions(std::ofstream& out_stream) {
	/* Modified 2013-4-1 STP
	 * Using setw(20) cannot handle longer arguments well. In particular, the
	 * output files are long strings and are being cut at 20 characters.
	 * I changed setw to using \t instead
	 */
	out_stream << "Using options specified by control file:" << std::endl;
	out_stream << std::endl << "# debug and loudness controls #" << std::endl;
	out_stream << "\tdebug\t\t\t" << debug << std::endl;
	out_stream << "\tseed\t\t\t" << seed << std::endl;

	out_stream << std::endl << "# control generations, frequency of sampling #"
			<< std::endl;
	out_stream << "\tgenerations\t\t\t" << gens << std::endl;

	out_stream << std::endl
			<< "# options for simulation to get proposal step in MCMC #"
			<< std::endl;

	out_stream << std::endl << "# probablity calculation method #" << std::endl;

	out_stream << std::endl << "# optional input file names #" << std::endl;
	out_stream << "\ttreefile\t\t\t" << treefile << std::endl;
	out_stream << "\tseqfile\t\t\t" << seqfile << std::endl;

	out_stream << std::endl << "# optional output file names #" << std::endl;
	out_stream << "\ttreeoutfile\t\t\t" << treeout << std::endl; // Validated
	out_stream << "\tsuboutfile\t\t\t" << subsout << std::endl; // Validated
	out_stream << "\tseqoutfile\t\t\t" << seqsout << std::endl;
	out_stream << "\tlikelihoodoutfile\t\t\t" << lnlout
			<< std::endl;
}

void Options::ConfigureOutputDirectoryNames() {
	/*
	 * Configures the output directory and the file names of the output files.
	 * Creates the output directory and changes the file names of the output file to
	 * include to full path to the output directory.
	 *
	 * For example: "seq.out" -> "/output_dir/seq.out"
	 */

	if (outdir == "") {
		time_t rawtime = time(NULL); // initializes rawtime
		struct tm * timeinfo = localtime(&rawtime); // transforms it into a local tm struct

		char dir[100];
		strftime(dir, 100, "%Y-%m-%d_%H%M%S/", timeinfo);
		outdir = std::string(dir);
	}

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
		std::cout << "Could not make output directory " << output_directory
				<< std::endl;
	} else {
		std::cout << "Making directory successful" << std::endl;
	}
#else	//ifdef __linux__ || __APPLE__
	struct stat st;
	if (stat(outdir.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
		if (not debug) {
			std::cout << "output dir " << outdir
			<< " exists. Overwrite? (Y/n)" << std::endl;
			if (getchar() != 'Y')
			exit(1);
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

	findFullFilePath(treeout);
	findFullFilePath(subsout);
	findFullFilePath(lnlout);
	findFullFilePath(seqsout);
}

void Options::findFullFilePath(std::string &parameter) {
	/*
	 * Prepends the output directory path to the output file names, giving the full path name
	 * for the given file.
	 */

	if (parameter == "") std::cerr << "Cannot prepend output directory to empty parameter" << std::endl;
	parameter = outdir + parameter;
}

void Options::debugint(bool debug, string blurb, int integer) {
    if (debug) std::cout << blurb << integer << std::endl;
}

void Options::InitializeRandomNumberGeneratorSeed() {
    string blurb = "Seed not specified so set to ";
    if (seed == 0) { // If seed not specified, set from clock
	    seed = time(0) ; 
	    debugint(debug,blurb,seed);
    }
    srand(seed);
}

void Options::CopyFile(string sourcefile, string newfile) {
	std::ifstream source(sourcefile.c_str());
	std::ofstream destination(newfile.c_str());
	destination << source.rdbuf();
}
