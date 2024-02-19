#pragma once

namespace Deltares {
    namespace ProbLibCore {

        enum class DPoptions
        {
            None,
            XfromU,
            XCorrelatedFromU,
            RMin,
            RMinZFunc,
            XCorrelatedFromUCompatible,
            RMinZFuncCompatible,
            MultiplyInvCholesky
        };
    }
}
