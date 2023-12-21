#include "runPy2Problib.h"
#include <pybind11/embed.h>
#include <iostream>
#include "pyRunLSF.h"
#include "../../../src/distributions/normalDist.h"
#include "../../../src/rootFinders/Anderson_Bjork.h"

using namespace::Deltares::ProbLibCore;

void runPy2Problib::run(const char* pyFile, const std::string & outFile, const runType type)
{
    std::ofstream file;
    if (outFile != "-")
    {
        file.open(outFile);
    }
    std::ostream & f = (outFile == "-" ? std::cout : file);

    py::module myzfunc = py::module::import(pyFile);

    py::object reader = myzfunc.attr("nstoch")();
    int nrStoch = reader.cast<int>();
    f << "nr of stochast(s) is: " << nrStoch << std::endl;

    auto pyRd = pyReader(f, myzfunc, nrStoch);
    listDistribs params;
    pyRd.read_params(params);

    auto calc = pyRd.readBasics();

    auto tableResults = std::vector<std::pair<double,double>>();
    auto runLSF = pyRunLSF(calc, pyRd.read_correlations(), f, type, pyFile, params);

    if (calc.design.type == designCalcType::design)
    {
        auto rootfinder = Anderson_Bjork(runLSF);
        auto x = rootfinder.FindRoot(calc.design.startValue, calc.design.stopValue);
        auto varname = params.getDistrib(calc.design.varid)->getName();
        f << std::endl << "Final result: " << varname << " = " << x.second << std::endl;
    }
    else
    {
        for (int i = 0; i < calc.design.loop(); i++)
        {
            auto r = runLSF.runLSF(i);
            tableResults.push_back({r.first, r.second.result.getBeta()});
        }
    }

    if (calc.design.type == designCalcType::table)
    {
        f << std::endl;
        auto varname = params.getDistrib(calc.design.varid)->getName();
        f << "step " << varname << " beta" << std::endl;
        for (size_t i = 0; i < tableResults.size(); i++)
        {
            f << i << " " << tableResults[i].first << " " << tableResults[i].second << std::endl;
        }
    }

    if (outFile != "-")
    {
        file.close();
    }
}
