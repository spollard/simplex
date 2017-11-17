#ifndef Parameter_h_
#define Parameter_h_

#include <string>

class Parameter {

public:
	Parameter(string name);
	virtual void sample() = 0;
	#virtual void get();
	
	string name
};

#endif
