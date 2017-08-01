#ifndef FloatParameter_h_
#define FloatParameter_h_


/**
 *
 *
 */

class FloatParameter: public Parameter {
	FloatParameter::FloatParameter(std::string name, float initial_value, float upper_bound=inf, float lower_bound=-inf) {	
		// How does inheritance setting variables work again?
		Parameter(name);
		value = initial_value;
	}
	
	void FloatParameter::sample() {
		// Sample the paramete
		return self.value // Set the value or return a new value?
		// if (Random() < 0.5 and value > step_size) {
			// value -= step_size;
		// } else {
			// value += step_size;
		// }
	}
			
	float value;

};

#endif
