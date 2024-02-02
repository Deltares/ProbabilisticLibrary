#include "../Deltares.Probabilistic.Kernels/Statistics/StandardNormal.h"
#include "stringHelper.h"

using namespace Deltares::Statistics;

extern "C"
void betafromqc(double * q, double * beta, tError * ierr)
{
    try
    {
        *beta = StandardNormal::getUFromQ(*q);
        ierr->errorCode = 0;
    }
    catch (const std::exception& e)
    {
        std::string s = e.what();
        fillErrorMessage(*ierr, s);
    }
}

extern "C"
double qfrombeta(double * beta)
{
    return StandardNormal::getQFromU(*beta);
}

extern "C"
double pfrombeta(double* beta)
{
    return StandardNormal::getPFromU(*beta);
}

extern "C"
void pqfrombeta(double* beta, double* p, double* q)
{
    *p = StandardNormal::getPFromU(*beta);
    *q = StandardNormal::getQFromU(*beta);
}

extern "C"
void returntimefrombeta(double *beta, double *returnTime)
{
    *returnTime =StandardNormal::ReturnTimeFromBeta(*beta);
}

extern "C"
void freqfrombeta(double *beta, double *freq)
{
    *freq = StandardNormal::FreqFromBeta(*beta);
}

extern "C"
void logqfrombeta(double *beta, double*logQ)
{
    *logQ = StandardNormal::LogQFromBeta(*beta);
}
