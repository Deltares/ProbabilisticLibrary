#pragma once

#include <memory>
#include "../../Deltares.Probabilistic/Model/Sample.h"
#include "../../Deltares.Probabilistic/Model/ModelRunner.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class testStartPointCalculator
            {
            public:
                void allStartPointTests();
            private:
                void testMethodOne();
                void zfunc(std::shared_ptr<Deltares::Models::Sample> sample);
                std::shared_ptr<Deltares::Models::ModelRunner> BuildProject();
            };
        }
    }
}
