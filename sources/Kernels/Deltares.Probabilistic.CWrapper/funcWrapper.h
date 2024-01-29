#pragma once
#include <functional>

#include "../Deltares.Probabilistic.Kernels/Model/Sample.h"
#include "stringHelper.h"

class funcWrapper
{
public:
    std::function<double(double[], int[], tError*)> zfunc;
    size_t allStoch;
    int* iPointer;
    double* xRef;

    double FDelegate(Sample* s);
}
;

