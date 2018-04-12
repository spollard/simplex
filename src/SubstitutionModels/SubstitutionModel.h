#ifndef SubstitutionModel_h_
#define SubstitutionModel_h_

#include <fstream>
#include <sstream>

#include <vector>
using std::vector;
#include <map>
using std::map;
#include <string>
using std::string;

#include "../Parameters/ParameterSet.h"

class SubstitutionModel {
	public:
		SubstitutionModel();
		virtual ~SubstitutionModel();
		virtual void Initialize(int number_of_sites, std::vector<std::string> states) = 0;

		void SampleParameters();
		void accept(); //After a model is sampled it must be accepted or rejected before next sampling.
		void reject();

		virtual void RecordState() = 0;
		virtual double SubstitutionProbability(int ancestral_state, int descendent_state, int site, double branch_length) = 0;
		virtual void Terminate();
	protected:
		int id;
		bool is_constant;
		std::ofstream* substitution_model_out;
		void Initialize();
		void InitializeState();
		virtual void InitializeStateFromFile(std::string state_in_file) = 0;
		std::string IdToString();
		ParameterSet parameters;
	private:
};

#endif
