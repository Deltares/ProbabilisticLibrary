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
    int computationId;         // reserved for e.g. wind direction
    int threadId;
};

typedef std::function<double(double[], computationSettings*, tError*)> zFuncExtern;

class funcWrapper
{
public:
    funcWrapper(const std::vector<int> & ip, const std::vector<double> & x, const int id, zFuncExtern func) :
        allStoch(x.size()), iPointer(ip), xRef(x), compId(id), zfunc(func) { ; }
    void FDelegate(std::shared_ptr<Deltares::Models::ModelSample> s);
    void FDelegateParallel(std::vector<std::shared_ptr<Deltares::Models::ModelSample>> s);
    void updateXinDesignPoint(double x[], double xx[]);
private:
    void copyXvector(double x[], const std::shared_ptr<Deltares::Models::ModelSample> s) const;
    void copyXvector(double x[], const double s[]) const;
    const size_t allStoch;
    const std::vector<int> & iPointer;
    const std::vector<double> & xRef;
    const int compId;
    zFuncExtern zfunc;
}
;

