#include <iostream>
#include "stringHelper.h"
#include "../../../src/correlation/HRcorrelation.h"
#include "../../../src/correlation/gaussianCorrelationRobust.h"

using namespace Deltares::ProbLibCore;

extern "C"
void calculatecorrelationc(double * u1, double * u2, corrParams * correlationParameters, bool * typeUCorrelation, tError * error)
{
    try
    {
        auto c = HRcorrelation();
        c.calculateCorrelation(*u1, *u2, *correlationParameters, *typeUCorrelation);
        error->errorCode = 0;
    }
    catch (const std::exception & e)
    {
        error->errorCode = -1;
        fillErrorMessage(*error, e.what());
    }
}

extern "C"
void correlationupdate_c(corrStruct * correlations, double* alpha_in, double* alpha_out, int nCor, int vectorSize)
{
    auto corr = std::vector<corrStruct>();
    for (int i = 0; i < nCor; i++)
    {
        auto p = corrStruct();
        p.correlation = correlations[i].correlation;
        p.idx1 = correlations[i].idx1 - 1;
        p.idx2 = correlations[i].idx2 - 1;
        corr.push_back(p);
    }
    auto gs = gaussianCorrelationRobust(vectorSize, corr);
    auto alpha = vector1D(vectorSize);
    for (int i = 0; i < vectorSize; i++)
    {
        alpha(i) = alpha_in[i];
    }
    gs.updateU(alpha);
    for (int i = 0; i < vectorSize; i++)
    {
        alpha_out[i] = alpha(i);
    }
}

