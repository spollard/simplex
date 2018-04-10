#include <iostream>
#include <cstdlib>

#include "SubstitutionModel.h"
#include "../Options.h"

#include "Types/GeneralTimeReversible.h"

extern Options options;

static const int general_time_reversible_model_type = 0;

SubstitutionModel* GetSubstitutionModel() {
	/*
	 * Given the global Options will dynamically attach the desired substitution model to the 
	 * substitution_model pointer. The model remains un initialized.
	 */

	SubstitutionModel* substitution_model = NULL;

	if (options.substitution_model_types.size() == 0) {
		std::cerr << "Not enough substitution models types given." << std::endl;
		exit(-1);
	}

	int next_model_type = options.substitution_model_types.front();
	options.substitution_model_types.pop();

	if (next_model_type == general_time_reversible_model_type) {
		substitution_model = new GeneralTimeReversible();
	} else {
		std::cerr << "Substitution model type not recognized in get next sub model " << next_model_type << std::endl;
		std::exit(-1);
	}
	return substitution_model;
}
