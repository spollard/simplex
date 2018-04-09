#include <iostream>

#include "Tree.h"
#include "Types/Tree_B1.h"

#include "Options.h"
extern Options options;

static const int normal_tree_type = 0;
static const int b1_tree_type = 1;

Tree* InstantiateTree() {
	Tree* tree = NULL;

	// This is a good candidate for a switch or case construction
	if (options.tree_type == normal_tree_type) {
		tree = new Tree();
	} else if (options.tree_type == b1_tree_type) {
		tree = new Tree_B1();
	} else {
		std::cerr
				<< "Tree type not recognized "
				<< options.tree_type << std::endl;
		std::exit(-1);
	}
	return tree;
}


