#ifndef GeneralTimeReversible_h_
#define GeneralTimeReversible_h_

#include <fstream>

#include "SubstitutionModel.h"
#include "../../Parameters/ParameterSet.h"
#include "../../Parameters/Types/ContinuousFloat.h"

class GeneralTimeReversible: public SubstitutionModel {
	public:
		GeneralTimeReversible();
		GeneralTimeReversible(const GeneralTimeReversible&);
		virtual ~GeneralTimeReversible();
		virtual GeneralTimeReversible* Clone();
		virtual void Initialize(int number_of_sites, std::vector<std::string> states);
		virtual void RecordState();
		virtual double SubstitutionProbability(int ancestral_state, int descendent_state, int site, double branch_length);
	private:
		static int number_of_single_probability_models;
		double substitution_probability;
		
		void InitializeOutputStream();
		virtual void InitializeStateFromFile(std::string state_in_file);
};

#endif
