#ifndef SIMPLEX_h_
#define SIMPLEX_h_

#ifdef _WIN32
#include <sys/time.h>
#else
#include <sys/times.h>
#endif

#include <iostream>
#include <string>
#include <cstdlib> // For rand
#include "Options.h"
#include "utils.h"

extern Options options;

namespace SimPLEX {
    void Terminate(time_t start_time);
}

// the time stuff ought to be a separate file
void SimPLEX::Terminate(time_t start_time) {
	time_t time_taken = time(NULL) - start_time;
	int h = time_taken / 3600;
	int m = (time_taken % 3600) / 60;
	int s = time_taken - (time_taken / 60) * 60;

	char result[100];
	if (h != 0) sprintf(result, "HH:MM:SS %d:%02d:%02d", h, m, s);
	else sprintf(result, "MM:SS %02d:%02d", m, s);

	std::cout << "Time taken: " << result << std::endl;
	std::string time_taken_file = "Time_taken";
	options.PrependOutputDirectory(time_taken_file); // ?
	std::ofstream time_out(time_taken_file.c_str());
	time_out << result << std::endl;

	std::cout << std::endl << "Output placed in " << options.outdir << std::endl;
}

#endif
