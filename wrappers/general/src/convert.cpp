#include "../../../src/probFuncs/conversions.h"
#include "stringHelper.h"

using namespace Deltares::ProbLibCore;

extern "C"
void betafromqc(double * q, double * beta, tError * ierr)
{
    try
    {
        *beta = conversions::betaFromQ(*q);
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
    return conversions::QfromBeta(*beta);
}

extern "C"
double pfrombeta(double* beta)
{
    return conversions::PfromBeta(*beta);
}

extern "C"
void pqfrombeta(double* beta, double* p, double* q)
{
    conversions::PQfromBeta(*beta, *p, *q);
}

extern "C"
void returntimefrombeta(double *beta, double *returnTime)
{
    *returnTime = conversions::ReturnTimeFromBeta(*beta);
}

extern "C"
void freqfrombeta(double *beta, double *freq)
{
    *freq = conversions::FreqFromBeta(*beta);
}

extern "C"
void logqfrombeta(double *beta, double*logQ)
{
    *logQ = conversions::LogQFromBeta(*beta);
}
