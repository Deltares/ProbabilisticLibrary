#include <iostream>
#include "stringHelper.h"
#include "../../../src/correlation/HRcorrelation.h"
#include "../../../src/correlation/gaussianCorrelationRobust.h"

using namespace Deltares::ProbLibCore;

extern "C"
void calculatecorrelationc(double * u1, double * u2, corrParams * correlationParameters, bool * typeUCorrelation, tError * error)
{
}

extern "C"
void correlationupdate_c(corrStruct * correlations, double* alpha_in, double* alpha_out, int nCor, int vectorSize)
{
}

