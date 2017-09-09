/*
 * Molecular Evolution Model Testing
 * This is for internal use only.
 *
 * Renamed from SimPLEX on 2014-2-13
 *
 * Author: Stephen Pollard
 * Stephen.T.Pollard@gmail.com
 * 2013-12-05
 */

#include "SimPLEX.h"
#include "Options.h"
#include "Data.h"

Options options;

int main() {
    options.defaultfile = "tests/Data_test.defaults"
    options.optionsfile = "test/Data_test.options"
	options.ReadOptions();

	Data data;
	data.Initialize();

    
    
	return 0;
}

