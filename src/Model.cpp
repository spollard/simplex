#include "Model.h"

#include <iostream>
#include <cmath>

#include "Trees/TreeTypes.h"
#include "SubstitutionModels/SubstitutionModelTypes.h"
#include "SubstitutionModels/SubstitutionModel.h"

#include "Options.h"

extern Options options;

/*  The tree needs the names_to_sequence map. The substitution model might need the empirical frequencies. */

using namespace std;

int Model::num_models = 0;

Model::Model() {
	/*
	 * The default contructor.
	 */
	id = num_models;
	num_models++;
	tree = NULL;
	SubstitutionModel* substitution_model = NULL;
}

Model::Model(const Model& model) {
	/*
	 *  A constructor that duplicates another model.
	 *  We should try to use this function usually, due to the expensive cost of duplicating
	 *  whole data structures.
	 */
	id = num_models;
	num_models++;

	tree = model.tree->Clone();
	substitution_model = model.substitution_model->Clone();
}

//Copy-swap
Model& Model::operator=(Model model) {
	/*
	 * The copy-swap assignment operator.
	 */
	std::swap(id, model.id);
	std::swap(tree, model.tree);
	std::swap(substitution_model, model.substitution_model);

	return (*this);
}

Model::~Model() {
	/*
	 * The destructor function.
	 * Note: we should not be destructing manually.
	 */
	delete tree;
	delete substitution_model;
}

SubstitutionModel* Model::InitializeSubstitutionModel(int num_sites, vector<string> states) {
	/*
	 * This is in the substituionModelType.h, can we make this more explicit?
	 */
	std::cout << "Start initialize." << std::endl;
	SubstitutionModel* substitution_model = GetNextSubstitutionModel(); // In SubstituionModelTypes.h
	std::cout << "Pointer: " << substitution_model << std::endl;
	substitution_model->RecordState();
	substitution_model->Initialize(num_sites, states);
	std::cout << "Successfully initialized model. " << substitution_model << std::endl;
	return(substitution_model);
}

void Model::Initialize(map<string, vector<int> > taxa_names_to_sequences, vector<string> states) {
	/*
	 * Initialize the model class.
	 * There are two main components within the model class:
	 * - the tree class - contains the tree topology as well as the sequences.
	 * - the substitution model class - which contains all the rate matrices.
	 */
	tree = InstantiateTree();
	tree->Initialize(taxa_names_to_sequences, states);

	int num_sites = taxa_names_to_sequences.begin()->second.size();
	substitution_model = InitializeSubstitutionModel(num_sites, states);
}

void Model::SampleParameters() {
	/*
	 * Samples both the parameters associated with the tree as well as the substitution model.
	 */
	tree->SampleParameters();
	substitution_model->SampleParameters();
}

void Model::RecordState() {
	/*
	 * Records the state of both the tree and the substitution model.
	 */
	tree->RecordState();
	substitution_model->RecordState();
}

double Model::CalcLnl() {
	/*
	 * Calculates the likelihood of the current tree and substitution model.
	 */
	return CalculateLogLikelihoodOfSubtree(*tree);
}

double Model::CalculateLogLikelihoodOfSubtree(Tree& tree) {
	/*
	 * Makes calculations for branches to children, then add calculation for children subtrees
	 *
	 * Note: This method for calculating the subtree does not result in correct values
	 * because the substitution mapping is too simple and incorrect. To properly
	 * calculate the likelihood, the substitution model must take into account
	 * all the possible substitution mappings that would begin with the ancestral
	 * state and end in the descendant state.
	 * */

	double Lnl = 0.0;
	if (tree.IsSubtree()) {  // required because children don't know their parent
		Lnl += CalculateLogLikelihoodOfChild(tree, *tree.left);
		Lnl += CalculateLogLikelihoodOfChild(tree, *tree.right);

		Lnl += CalculateLogLikelihoodOfSubtree(*tree.left);
		Lnl += CalculateLogLikelihoodOfSubtree(*tree.right);
	}  return Lnl;
}

double Model::CalculateLogLikelihoodOfChild(Tree& tree, Tree& child) {
	/*
	 * foreach site k, get substitution probability from seq state at k in parent to child,
	 * over distance (attached to child).
	 *
	 * Note: this needs to be changed for true plex calculation, and avoid log every time
	 */
	double Lnl = 0.0;
	for (int site = 0; site < tree.sequence.size(); site++) {
		substitution_model->RecordState();
		Lnl += log(substitution_model->SubstitutionProbability(tree.sequence.at(site), child.sequence.at(site), site, child.distance));
	}  return Lnl;
}

void Model::Terminate() {
	/*
	 * Just terminates substitution_model.
	 */
	substitution_model->Terminate();
}
