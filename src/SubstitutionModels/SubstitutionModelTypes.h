#ifndef SubstitutionModelTypes_h_
#define SubstitutionModelTypes_h_

#include "SubstitutionModel.h"

#include "Types/SingleProbabilityModel.h"
#include "Types/SingleSubstitutionRateModel.h"
#include "Types/ProbabilityMatrixModel.h"
#include "Types/SubstitutionRateMatrixModel.h"
#include "Types/RadiusDependentModel.h"
#include "Types/SubstitutionMixtureModel.h"

#include "../Options.h"

/**
 * I'm not sure of the best way to do this. These are not globals; they only
 * are needed in two files: Model.cpp and SubsitutionMixtureModel.cpp .
 *
 * And now RadiusDependentModel.cpp.
 *
 * I might use enum's?
 *
 */

SubstitutionModel* GetNextSubstitutionModel();

#endif
