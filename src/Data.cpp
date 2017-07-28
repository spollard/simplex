#include "Data.h"

#include <algorithm>

#include "Options.h"

extern Options options;

extern const int gap_indicator = -1;

Data::Data() {
//	cout << "Data constructor" << endl;
}

Data::~Data() {
//	cout << "Data destructor" << endl;
}

void Data::Initialize() {
	ReadSequences();
}

/// This function is too long.
void Data::ReadSequences() {
	// This is the only dependency on options.
	ifstream sequences_in(options.sequences_file.c_str());

	if (not sequences_in.good()) {
		cerr << "Cannot read sequence file" << endl;
		exit(-1);
	}

	vector<int> encoded_sequence;
	string line;
	string sequence = "";
	string name;

	while (sequences_in.good()) {
		getline(sequences_in, line);
		//STP: Remove spaces and concatenate words
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		//STP: This removes the carriage return in Windows files
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

		if (line == "")
			continue;

		if (line.at(0) == '>') {
			if (sequence != "") {
				taxa_names_to_sequences[name] = EncodeSequence(
						sequence);
			}
			name = line.substr(1);
			sequence = "";
		} else {
			sequence += line;
		}
	}
// Add final sequence
	taxa_names_to_sequences[name] = EncodeSequence(sequence);
}

vector<int> Data::EncodeSequence(string sequence) {
	vector<int> encoded_sequence(sequence.length());

	for (int site = 0; site < sequence.length(); site++) {
		string state = sequence.substr(site, 1);
		if (HasGap(state)) {
			encoded_sequence.at(site) = gap_indicator;
		} else {
			AddStateToStates(state);
			encoded_sequence.at(site) = state_to_integer[state];
		}
	}
	return encoded_sequence;
}

bool Data::HasGap(string sequence) {
	// When sequence.find == string::npos, the string was not found
	return (sequence.find('-') != string::npos
			or sequence.find('?') != string::npos);
}

void Data::AddStateToStates(string state) {
	if (state_to_integer.find(state) == state_to_integer.end()) {
		int encoded_state = state_to_integer.size();
		state_to_integer[state] = encoded_state;

		states.push_back(state);
	}
}

void Data::PrintTaxaAndSequences() {
	for (std::map<string, vector<int> >::iterator it =
			taxa_names_to_sequences.begin();
			it != taxa_names_to_sequences.end(); ++it) {
		cout << it->first << ":";
		for (int i = 0; i < it->second.size(); i++)
			cout << " " << it->second.at(i);
		cout << endl;
	}
}
