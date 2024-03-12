#pragma once
#include <functional>
#include <memory>

#include "../Deltares.Probabilistic/Model/ModelSample.h"
#include "stringHelper.h"

typedef std::function<double(double[], int[], tError*)> zFuncExtern;
class funcWrapper
{
public:
    funcWrapper(const size_t nrStoch, int* ip, double* x, int* ids, zFuncExtern func) :
        allStoch(nrStoch), iPointer(ip), xRef(x), compIds(ids), zfunc(func) { ; }
    void FDelegate(std::shared_ptr<Deltares::Models::ModelSample> s);
private:
    size_t allStoch;
    int* iPointer;
    double* xRef;
    int* compIds;
    zFuncExtern zfunc;
}
;

