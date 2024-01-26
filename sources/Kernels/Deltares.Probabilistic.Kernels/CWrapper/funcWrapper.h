#pragma once
#include <functional>

#include "../Model/Sample.h"
#include "stringHelper.h"

class funcWrapper
{
public:
    std::function<double(double[], int[], tError*)> staticF;
    size_t allStoch;
    int* iPointer;
    double* xRef;

    double FDelegate(Sample* s);
}
;

