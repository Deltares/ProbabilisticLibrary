#pragma once
#include "../../src/probMethods/zmodel.h"
#include "../../src/probFuncs/probDraw.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        class mhwtestz : public zmodel
        {
        public:
            mhwtestz(const double value) : refValue(value) {};
            mhwtestz(const double value, const size_t nrThreads) : zmodel(nrThreads), refValue(value) {};
            double zfunc(const probDraw& d) const;
        private:
            const double refValue;
        };

        class mhwtest
        {
        public:
            void test1() const;
            void test2() const;
            void test3() const;
        };
    }
}
