#ifndef TreeTypes_h_
#define TreeTypes_h_

#include "Types/Tree.h"
#include "Types/Tree_B1.h"

#include "Environment.h"

extern Environment env;

/**
 * I'm not sure of the best way to do this. These are not globals; they only
 * are needed in one file: Model.cpp
 *
 */

Tree* InstantiateTree();

#endif
