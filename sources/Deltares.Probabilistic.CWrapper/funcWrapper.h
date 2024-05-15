#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "../Deltares.Probabilistic/Model/ModelSample.h"
#include "stringHelper.h"

enum class designPointOptions
{
    dpOutFALSE = 0,
    dpOutTRUE = 1,
    dpOutPrintAll = 3,
};

struct computationSettings
{
    designPointOptions dpOut;
    int computationId;         // reserved for e.g. wind direction
    int threadId;
};

typedef std::function<double(double[], computationSettings*, tError*)> zFuncExtern;

class funcWrapper
{
public:
    funcWrapper(const int id, zFuncExtern func) : compId(id), zfunc(func) { ; }
    void FDelegate(std::shared_ptr<Deltares::Models::ModelSample> s);
    void FDelegateParallel(std::vector<std::shared_ptr<Deltares::Models::ModelSample>> s);
    void updateXinDesignPoint(std::vector<double> & x);
private:
    const int compId;
    zFuncExtern zfunc;
}
;

