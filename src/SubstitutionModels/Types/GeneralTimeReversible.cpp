#include "GeneralTimeReversible.h"

#include <iostream>
#include <cstdlib>

#include "Options.h"

extern Options options;

extern double Random();

int GeneralTimeReversible::number_of_single_probability_models = 0;

GeneralTimeReversible::GeneralTimeReversible() {
	std::cout << "In specific model." << std::endl;
	id = number_of_single_probability_models;
	number_of_single_probability_models++;

	substitution_probability = 0;
}

GeneralTimeReversible::GeneralTimeReversible(const GeneralTimeReversible& GTR_model) : SubstitutionModel(GTR_model) {
		substitution_probability = GTR_model.substitution_probability;
}

GeneralTimeReversible::~GeneralTimeReversible() {
	/*
	 * std::cout << "Single probability model deconstructor" << std::endl;
	 * delete substitution_model_out;
	 */
}

GeneralTimeReversible* GeneralTimeReversible::Clone() {
	/*
	 * std::cout << "cloning single probability model" << std::endl;
	 */
	return new GeneralTimeReversible(*this);
}

void GeneralTimeReversible::Initialize(int number_of_sites, std::vector<std::string> states) {
	/*
	 * std::cout << "Initializing Single Probability Model" << std::endl;
	 */
	std::cout << "Initializing General Time Reversible Model (GTR)." << std::endl;
	SubstitutionModel::Initialize(); // sets is_constant

	InitializeState(); // Can call InitializeStateFromFile()
	InitializeOutputStream();
	if (is_constant) {
		is_constant = false;
		RecordState();
		is_constant = true;
	}
}

void GeneralTimeReversible::InitializeStateFromFile(std::string state_in_file) {
	std::ifstream state_in(state_in_file.c_str());

	std::string header;
	state_in >> header;
	if (header != "Substitution_Probability") {
		std::cerr << "Check header on substitution probability model initialization file for model " << IdToString() << std::endl;
		std::cerr << "The header should be \"Substitution_Probability\"" << std::endl;
		std::exit(-1);
	}

	state_in >> substitution_probability;
}

void GeneralTimeReversible::InitializeOutputStream() {
	std::string substitution_model_out_file = "single_probability_model_"
			+ IdToString();
	options.findFullFilePath(substitution_model_out_file);
	substitution_model_out = new std::ofstream(
			substitution_model_out_file.c_str());
	*substitution_model_out << "Substitution_Probability" << std::endl;
}

void GeneralTimeReversible::SampleParameters() {
	/*
	 * std::cout << "Sampling single probability model parameters" << std::endl;
	 */
	if (not is_constant) {
		substitution_probability = Random();
	}
}

void GeneralTimeReversible::RecordState() {
	if (not is_constant) {	 
		//*substitution_model_out << substitution_probability << std::endl;
	}
}

double GeneralTimeReversible::SubstitutionProbability(int ancestral_state, int descendent_state, int site, double branch_length) {

	double probability = 0;

	if (ancestral_state != descendent_state)
		probability = substitution_probability;
	else
		probability = 1 - substitution_probability;

	return probability;
}

