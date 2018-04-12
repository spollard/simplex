#include "ParameterSet.h"

ParameterSet::ParameterSet() {
	/*
	 * The default parameter set constructor.
	 */
}

void ParameterSet::Initialize() {
	current_parameter = parameter_list.begin();
}

void ParameterSet::add_parameter(AbstractParameter* param) {
	/* 
	 * Adds the pointer to an actual parameter onto the parameter_list, and to the
	 * name_to_adress map.
	 */
	parameter_list.push_back(param);

	std::string name = param->name;
	name_to_address.insert(std::make_pair(name, param));
}

void ParameterSet::sample() {
	/*
	 * Will sample the current parameters.
	 */
	(*current_parameter)->sample();
	
}

void ParameterSet::stepToNextParameter() {
	/*
	 * Sets the current_parameter iterator to the next sample.
	 */
	++current_parameter;
	if(current_parameter == parameter_list.end()) {
		current_parameter = parameter_list.begin();
	}
}

void ParameterSet::accept() {
	(*current_parameter)->fix();
	stepToNextParameter();
}

void ParameterSet::reject() {
	(*current_parameter)->undo();
	stepToNextParameter();
}

void ParameterSet::print() {
	/*
	 * Prints a short description of the state of the parameter_list.
	 */
	std::cout << "Parameter Set - size: " << parameter_list.size() << std::endl;
	std::list<AbstractParameter*>::iterator iter = parameter_list.begin();
	while(iter != parameter_list.end()) {
		(*iter)->printValue();
		++iter;
	}
}

double ParameterSet::get(const std::string& name) {
	/*
	 * Will retreive the value of a parameter from the parameter set.
	 */
	return name_to_address[name]->getValue();
}




