#pragma once
#include <memory>
#include "../../../src/probFuncs/listDistribs.h"
#include "../../../src/correlation/correlation.h"
#include "../../../src/rootFinders/root_finder.h"
#include "pyReader.h"

using namespace::Deltares::ProbLibCore;

class pyRunLSF : public FuncWrapper
{
public:
    pyRunLSF(const tPyCalc & pc, correlation* idPtr, std::ostream & out, const runType tp,
      const char* py, listDistribs & pars);
    std::pair<double,lsfResult> runLSF(const int i);
    std::pair<double,lsfResult> runLSF(double x);
    virtual double GetValue(const double x);
private:
    const tPyCalc & calc;
    std::shared_ptr<correlation> id;
    std::ostream & o;
    const runType type;
    const char* pyFile;
    listDistribs & params;
};
