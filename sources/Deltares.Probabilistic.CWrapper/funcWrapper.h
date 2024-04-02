#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "../Deltares.Probabilistic/Model/ModelSample.h"
#include "stringHelper.h"

enum class designPointOptions
{
    dpOutTRUE = 0,
    dpOutFALSE = 1,
    dpOutPrintAll = 3,
};

struct computationSettings
{
    designPointOptions dpOut;
    int computationId;
    int threadId;
};

typedef std::function<double(double[], computationSettings*, tError*)> zFuncExtern;

class funcWrapper
{
public:
    funcWrapper(const size_t nrStoch, int* ip, double* x, int* ids, zFuncExtern func) :
        allStoch(nrStoch), iPointer(ip), xRef(x), compIds(ids), zfunc(func) { ; }
    void FDelegate(std::shared_ptr<Deltares::Models::ModelSample> s);
    void FDelegateParallel(std::vector<std::shared_ptr<Deltares::Models::ModelSample>> s);
    void updateXinDesignPoint(double x[], const size_t lenX);
private:
    void copyXvector(double x[], const std::shared_ptr<Deltares::Models::ModelSample> s) const;
    void copyXvector(double x[], const double s[], const size_t lenX) const;
    size_t allStoch;
    int* iPointer;
    double* xRef;
    int* compIds;
    zFuncExtern zfunc;
}
;

