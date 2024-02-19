#pragma once
#include <vector>
#include "../../../src/probFuncs/stochSettings.h"
#include "../../../src/probMethods/parseAndRunMethod.h"
#include "../../../src/correlation/correlation.h"
#include "../../../src/probFuncs/listDistribs.h"
#include <pybind11/embed.h>

using namespace::Deltares::ProbLibCore;

namespace py = pybind11;

enum class designCalcType
{
    normal,
    table,
    design,
};

class tDesign
{
public:
    designCalcType type;
    double startValue;
    double stopValue;
    double step;
    double beta;
    size_t varid;
    int loop() const
    {
        return (type == designCalcType::table ? 1 + (int)((stopValue - startValue)/step): 1);
    }
};

struct tPyCalc
{
    tDesign design;
    ProbMethod method;
    std::string tree;
    basicSettings settings;
};

class pyReader
{
public:
    pyReader(std::ostream & f, py::module & myzfunc, const int nrStoch);
    tPyCalc readBasics();
    void read_params(listDistribs & ld);
    correlation* read_correlations();
private:
    std::ostream & o;
    py::module & myzfunc;
    const int nrStoch;
    basicSettings read_settings(const ProbMethod m);
    std::string read_tree();
    tDesign read_design();
};
