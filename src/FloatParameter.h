#ifndef FloatParameter_h_
#define FloatParameter_h_

#include <string>

class FloatParameter: public Parameter {

public:
	FloatParameter(string name, float initial_value, float upper_bound, float lower_bound);
	void sample();

	float value;
};

#endif
