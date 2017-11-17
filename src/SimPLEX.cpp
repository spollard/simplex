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
#include "Model.h"
#include "Data.h"
#include "MCMC.h"
#include "Main.h"

#ifdef _WIN32
#include <sys/time.h>
#else
#include <sys/times.h>
#endif

Options options;

int main() {
	time_t start_time = time(NULL);

	Main::Initialize();

	Data data;
	data.Initialize();

	Model model;
	model.Initialize(data.taxa_names_to_sequences, data.states);

	MCMC mcmc;
	mcmc.Init(&model);

	mcmc.Run();

	// Now that I have objects allocated on the heap, this is required. Unless I use shared pointers...
	model.Terminate();
	Main::Terminate(start_time);

	return 0;
}

