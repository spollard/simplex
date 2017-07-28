
#ifndef ObservedData_h_
#define ObservedData_h_


#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <set> // for columns_with_gaps
#include <map> // for taxa_names_to_sequences
#include <algorithm>

using namespace std;

class Data {
public:

	// Don't need to initialize the following because they are not primitives.
	// Their default constructors are called.
	map<string, vector<int> > taxa_names_to_sequences;

	map<string, int> state_to_integer;
	vector<string> states;

	Data();
	~Data();

	void Initialize();

private:
	set<int> columns_with_gaps;
	vector<int> columns_without_gaps;

	void ReadSequences();

	vector<int> EncodeSequence(string);
	bool HasGap(string);
	void AddStateToStates(string state);

	void PrintTaxaAndSequences();
};

#endif

