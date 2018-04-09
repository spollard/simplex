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

int Model::num_models = 0; // what is best way to go through models?

// constructor creates tree, substitution_model
Model::Model() {   //	cout << "Model default constructor" << endl;
	id = num_models;
	num_models++;
	tree = NULL;
	SubstitutionModel* substitution_model = NULL;
}

// copy; we want to avoid this, which creates new model and clones the structures
// STP: Must include a virtual clone method for derived classes to return a clone of themselves.
Model::Model(const Model& model) {
	/*
	 *  A constructor that duplicates another model.
	 */
	// cout << "Model copy constructor" << endl;
	id = num_models;
	num_models++;

	tree = model.tree->Clone();
	substitution_model = model.substitution_model->Clone();
}

//Copy-swap
Model& Model::operator=(Model model) {
	std::swap(id, model.id);
	std::swap(tree, model.tree);
	std::swap(substitution_model, model.substitution_model);

	return (*this);
}

Model::~Model() {
	/*
	 * The destructor function.
	 * Note: we should not be destructing
	 */
	// cout << "Model destructor" << endl;
	delete tree;
	delete substitution_model;
}

void Model::Initialize(map<string, vector<int> > taxa_names_to_sequences,
		       vector<string> states) {

	tree = InstantiateTree();
	tree->Initialize(taxa_names_to_sequences, states);

	int num_sites = taxa_names_to_sequences.begin()->second.size();
	InitializeSubstitutionModel(num_sites, states);
}

void Model::InitializeSubstitutionModel(int num_sites, vector<string> states) {
	 //This is in the substituionModelType.h, can we make this more explicit?
	std::cout << "Start initialize." << substitution_model << std::endl;
	substitution_model = GetNextSubstitutionModel();
	std::cout << "Pointer: " << substitution_model << std::endl;
	substitution_model->RecordState();
	substitution_model->Initialize(num_sites, states);
	std::cout << "Successfully initialized model. " << substitution_model << std::endl;
}

/**
 *  What if I want the model to be constant? There's not much use for a model
 *  to be constant in an MCMC context. But I might want to use the MCMC to
 *  simply calculate the likelihood. or rather, I might want to use SimPLEX
 *  to just calculate the likelihood of a tree and sequences given a model,
 *  including a substitution model and a tree with the ancestors inferred.
 */

/**
 * Any sort of interaction between the tree and substitution model sampling
 * would happen here. For example, if one wants to use Gibbs sampling for the
 * ancestral states, one would need to know about the substitution model when
 * sampling the states. The only class that knows about both the substitution
 * model and the tree states is the Model class.
 */

void Model::SampleParameters() { //	std::cout << "Sampling model parameters" << std::endl;
	tree->SampleParameters();
	substitution_model->SampleParameters();
}

void Model::RecordState() {
	tree->RecordState();
	substitution_model->RecordState();
}

double Model::CalcLnl() {
	return CalculateLogLikelihoodOfSubtree(*tree);
}

/**
 * This method for calculating the subtree does not result in correct values
 * because the substitution mapping is too simple and incorrect. To properly
 * calculate the likelihood, the substitution model must take into account
 * all the possible substitution mappings that would begin with the ancestral
 * state and end in the descendant state.
 */

// make calculations for branches to children, then add calculation for children subtrees
double Model::CalculateLogLikelihoodOfSubtree(Tree& tree) {
	double Lnl = 0.0;
	if (tree.IsSubtree()) {  // required because children don't know their parent
		Lnl += CalculateLogLikelihoodOfChild(tree, *tree.left);
		Lnl += CalculateLogLikelihoodOfChild(tree, *tree.right);

		Lnl += CalculateLogLikelihoodOfSubtree(*tree.left);
		Lnl += CalculateLogLikelihoodOfSubtree(*tree.right);
	}  return Lnl;
}

// foreach site k, get substitution probability from seq state at k in parent to child, over distance (attached to child)
// this needs to be changed for true plex calculation, and avoid log every time
double Model::CalculateLogLikelihoodOfChild(Tree& tree, Tree& child) {
	double Lnl = 0.0;
	for (int site = 0; site < tree.sequence.size(); site++) {
		substitution_model->RecordState();
		Lnl += log(substitution_model->SubstitutionProbability(tree.sequence.at(site),
								       child.sequence.at(site),
								       site,
								       child.distance));
	}  return Lnl;
}

// model Terminate() just terminates substitution_model
void Model::Terminate() {   //tree->terminate
	substitution_model->Terminate();
}
