#pragma once
#include "../../src/probMethods/zmodel.h"
#include "../../src/rnd/defaultRandomDouble.h"
#include <msclr/auto_gcroot.h>

using namespace System;
using namespace Deltares::Reliability;
using namespace Deltares::Mathematics;
using namespace Deltares::ProbLibCore;

namespace Deltares {
    namespace Reliability {
        namespace Wrapper {
            public class rndDotNet : public defaultRandomDouble
            {
            public:
                rndDotNet(ModelRunner^ m) : mr(m) {};
                virtual void getRandomVector(vector1D& r);
            private:
                msclr::auto_gcroot <ModelRunner^> mr;
            };

            public class zmodelDotNet : public zmodel
            {
            public:
                zmodelDotNet(ModelRunner^ m) : mr(m) {};
                double zfunc(const probDraw& d) const;
            private:
                msclr::auto_gcroot <ModelRunner^> mr;
            };

            public ref class CrudeMC
            {
            public:
                CrudeMC(MonteCarloSettings^ settings) : mcSettings(settings) {};
                double getDesignPoint(ModelRunner ^ m);
            private:
                MonteCarloSettings^ mcSettings;
            };
        }
    }
}
