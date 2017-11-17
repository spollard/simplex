#ifndef MAIN_h_
#define MAIN_h_

#ifdef _WIN32
#include <sys/time.h>
#else
#include <sys/times.h>
#endif

#include <iostream>
#include <string>
#include <cstdlib> // For rand

namespace Main {
    void Initialize();
    void Terminate(time_t);
    void PrintSimPLEXHeader();
}

#endif

