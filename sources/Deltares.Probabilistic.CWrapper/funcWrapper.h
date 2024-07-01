#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "../Deltares.Probabilistic/Model/ModelSample.h"
#include "../Deltares.Probabilistic/Model/RunSettings.h"
#include "stringHelper.h"

struct computationSettings
{
    Deltares::Models::designPointOptions dpOut;
    int computationId;         // reserved for e.g. wind direction
    int threadId;
    int reliabilityMethodSubStepsCounter;
};

typedef std::function<double(double[], computationSettings*, tError*)> zFuncExtern;

class funcWrapper
{
public:
    funcWrapper(const int id, zFuncExtern func) : compId(id), zfunc(func) { ; }
    void FDelegate(std::shared_ptr<Deltares::Models::ModelSample> s);
    void FDelegateParallel(std::vector<std::shared_ptr<Deltares::Models::ModelSample>> s);
    std::vector<std::string> error_messages;
private:
    const int compId;
    zFuncExtern zfunc;
}
;

