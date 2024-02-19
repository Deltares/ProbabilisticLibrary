#include "pyRunLSF.h"
#include <memory>
#include "zfuncwrapper.h"
#include "../../../src/distributions/normalDist.h"

using namespace::Deltares::ProbLibCore;

pyRunLSF::pyRunLSF(const tPyCalc & pc, correlation* idPtr, std::ostream & out, const runType tp,
  const char* py, listDistribs & pars)
  : calc(pc), id(idPtr), o(out), type(tp), pyFile(py), params(pars)
{
}

std::pair<double,lsfResult> pyRunLSF::runLSF(const int i)
{
    auto x = calc.design.startValue + calc.design.step * (double) i;
    return runLSF(x);
}

double pyRunLSF::GetValue(const double x)
{
    auto r = runLSF(x);
    return r.second.result.getBeta() - calc.design.beta;
}

std::pair<double,lsfResult> pyRunLSF::runLSF(double x)
{
    const size_t nrStoch = params.size();

    if (calc.design.type != designCalcType::normal)
    {
        auto name = params.getDistrib(calc.design.varid)->getName();
        auto s = new deterministicDist(name, x);
        params.updateDistrib(calc.design.varid, s);
    }

    auto s = stochSettings(*id, params, true);
    lsfResult result;
    auto pg = progress(calc.settings.progressInterval, o);
    if (calc.tree == "")
    {
        auto w = zfuncwrapper(pyFile, nrStoch, "z");
        auto pr = parseAndRunMethod(calc.settings, pg);
        result = pr.parseRunAndPrint(calc.method, w, s, o);
    }
    else
    {
        auto ws = std::vector<std::unique_ptr<zmodel>>();
        auto z1 = calc.tree.substr(0, 2);
        auto z2 = calc.tree.substr(calc.tree.size()-2,2);
        std::unique_ptr<zmodel> p1 (new zfuncwrapper(pyFile, nrStoch, z1.c_str()));
        std::unique_ptr<zmodel> p2 (new zfuncwrapper(pyFile, nrStoch, z2.c_str()));
        ws.push_back(std::move(p1));
        ws.push_back(std::move(p2));
        auto pr = parseAndRunMethod(calc.settings, pg);
        auto results = pr.parseAndRunTree(calc.method, ws, s, calc.tree, type, o);
        result = results[2];
    }
    return {x, result};
}
