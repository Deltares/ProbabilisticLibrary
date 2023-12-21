#include "../../../src/rnd/george_marsaglia.h"

using namespace Deltares::ProbLibCore;

// note: only one instance of the generator
// that suffices for testing the properties.
static george_marsaglia* gm;

extern "C"
void initrandomgenerator(const int ij, const int kl)
{
    gm = new george_marsaglia(ij, kl);
}

extern "C"
void getrandomvalue(double* randomValue)
{
    *randomValue = gm->get_uniform();
}

