#pragma once
#include "../../src/utils/matrix.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        class testdata
        {
        public:
            static Matrix getTable();
            static Matrix getStatUnc();
            static Matrix getStatUncBeta();
        };
    }
}
