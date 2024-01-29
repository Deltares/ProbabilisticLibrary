#pragma once
#include <functional>

#include "../Deltares.Probabilistic.Kernels/Model/Sample.h"
#include "stringHelper.h"

typedef std::function<double(double[], int[], tError*)> zFuncExtern;
class funcWrapper
{
public:
    funcWrapper(const size_t nrStoch, int* ip, double* x, zFuncExtern func) :
        allStoch(nrStoch), iPointer(ip), xRef(x), zfunc(func) {;}
    double FDelegate(Sample* s);
private:
    size_t allStoch;
    int* iPointer;
    double* xRef;
    zFuncExtern zfunc;
}
;

