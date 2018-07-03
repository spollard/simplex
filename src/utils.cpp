#include "utils.h"
#include <iostream>
#include <stdlib.h>
#include "Environment.h"

extern Environment env;

void utils::printHeader() {
	std::cout << std::endl << "SimPLEX" << std::endl
		<< "by Stephen T. Pollard" << std::endl
		<< "stephen.pollard@ucdenver.edu" << std::endl
		<< "For internal use only." << std::endl
		<< std::endl;
}

void utils::Terminate(time_t start_time) {
	time_t time_taken = time(NULL) - start_time;
	int h = time_taken / 3600;
	int m = (time_taken % 3600) / 60;
	int s = time_taken - (time_taken / 60) * 60;

	char result[100];
	if (h != 0) sprintf(result, "HH:MM:SS %d:%02d:%02d", h, m, s);
	else sprintf(result, "MM:SS %02d:%02d", m, s);

	std::cout << "Time taken: " << result << std::endl;
	std::string time_taken_file = "Time_taken";
	time_taken_file = env.findFullFilePath(time_taken_file);
	std::ofstream time_out(time_taken_file.c_str());
	time_out << result << std::endl;

	std::cout << std::endl << "Output placed in " << env.outdir << std::endl;
}

