#ifndef FloatParameter_h_
#define FloatParameter_h_


/**
 * This substitution model has a substitution rate for a substitution between an
 * ancestral state and a descendant state.
 *
 * Stephen Pollard
 * 12/22/2013
 *
 *
 */

class FloatParameter: public Parameter {
	FloatParameter::FloatParameter(string name, float initial_value, float upper_bound=inf, float lower_bound=-inf) {
		
	}
	void FloatParameter::sample() {
		if (Random() < 0.5 and substitution_rate > step_size) {
			substitution_rate -= step_size;
		} else {
			substitution_rate += step_size;
		}
	}
			
	float value;

};

#endif
