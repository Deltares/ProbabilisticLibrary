#include "../Deltares.Probabilistic/Math/Randomizers/GeorgeMarsagliaRandomValueGenerator.h"

//using namespace Deltares::ProbLibCore;

// note: only one instance of the generator
// that suffices for testing the properties.
static Deltares::Numeric::GeorgeMarsagliaRandomValueGenerator* gm;

extern "C"
void initrandomgenerator(const int ij, const int kl)
{
    gm = new Deltares::Numeric::GeorgeMarsagliaRandomValueGenerator();
    gm->initialize(true, ij, kl);
}

extern "C"
void getrandomvalue(double* randomValue)
{
    *randomValue = gm->next();
}

