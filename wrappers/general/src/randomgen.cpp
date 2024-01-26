#include "../../../../Kernels/Deltares.Probabilistic.Kernels/Math/Randomizers/GeorgeMarsagliaRandomValueGenerator.h"

//using namespace Deltares::ProbLibCore;

// note: only one instance of the generator
// that suffices for testing the properties.
static GeorgeMarsagliaRandomValueGenerator* gm;

extern "C"
void initrandomgenerator(const int ij, const int kl)
{
    gm = new GeorgeMarsagliaRandomValueGenerator();
    gm->initialize(true, ij, kl);
}

extern "C"
void getrandomvalue(double* randomValue)
{
    *randomValue = gm->next();
}

