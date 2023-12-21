#pragma once
#include <pybind11/embed.h>
#include "../../../src/probMethods/zmodel.h"
#include "../../../src/probFuncs/probDraw.h"

using namespace::Deltares::ProbLibCore;

class zfuncwrapper : public zmodel
{
public:
    zfuncwrapper(const char * pyMod, const size_t n, const char* nameZfunc );
    virtual double zfunc(const probDraw & d) const;
private:
    const char* pyModFile;
    size_t nrStoch;
    const char* nameZ;
    pybind11::object func;
};
