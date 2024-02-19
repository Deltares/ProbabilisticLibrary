#pragma once
#include <vector>
#include "stringHelper.h"
#include "../../../src/probMethods/zmodel.h"

using namespace Deltares::ProbLibCore;

class fortranZfunc : public zmodel
{
public:
    fortranZfunc(const double(*f)(double[], int[], tError*), const size_t n, const size_t vectorSize,
      int compIds[], int iPoint[], double x[], const size_t maxThreads);
    ~fortranZfunc();
    virtual double zfunc(const probDraw & d) const;
    virtual double zfunc(const probDraw & d, const int threadId) const;
    virtual double zfunc(const probDraw& d, const OutputFlags option, vector1D& x) const;
    double zfunc(const vector1D & v) const;
    virtual void logInfo(const int id) const { ids[1] = id; }
private:
    const double(*fx)(double[], int[], tError*);
    const size_t nrStoch;
    std::vector<double *>x;
    int* ids;
    int* ipoint;
};
