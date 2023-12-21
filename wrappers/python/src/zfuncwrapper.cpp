#include "zfuncwrapper.h"
#include "../../../src/utils/probLibException.h"
#include <pybind11/stl.h>  // type conversion

using namespace::Deltares::ProbLibCore;

namespace py = pybind11;

zfuncwrapper::zfuncwrapper(const char * pyMod, const size_t n, const char* nameZfunc)
  : pyModFile(pyMod), nrStoch(n), nameZ(nameZfunc)
{
    py::module myzfunc = py::module::import(pyModFile);
    func = myzfunc.attr(nameZ);
};

double zfuncwrapper::zfunc(const probDraw & d) const
{
        auto x = std::vector<double>();
        for (size_t i = 0; i < nrStoch; i++)
        {
            x.push_back(d.getx(i));
        }
        py::object result = func(x);
        return result.cast<double>();
}
