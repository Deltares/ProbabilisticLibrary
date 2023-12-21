#pragma once
#include "../../src/probMethods/zmodel.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        class waarts1 : public zmodel
        {
        public:
            waarts1() {};
            waarts1(const size_t numcores) : zmodel(numcores) {};
            double zfunc(const probDraw& d) const;
        };

        class simple : public zmodel
        {
            double zfunc(const probDraw& d) const;
        };
    }
}
