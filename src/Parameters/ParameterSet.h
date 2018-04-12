#ifndef ParameterSet_h_
#define ParameterSet_h_

#include <list>
#include <map>
#include <string>
#include <iostream>

#include "AbstractParameter.h"

class ParameterSet {
	public:
		ParameterSet();
		void Initialize();
		void add_parameter(AbstractParameter*);

		void sample();
		void accept();
		void reject();

		void print();
		double get(const std::string&);
	private:
		void stepToNextParameter();

		std::list<AbstractParameter*> parameter_list;
		std::list<AbstractParameter*>::iterator current_parameter; //Tracks the current parameter to be sampled, via an iterator across the parameter_list.
		std::map<std::string, AbstractParameter*> name_to_address; //A map from the name of a parameter to the pointer of the parameter class.
};

#endif
