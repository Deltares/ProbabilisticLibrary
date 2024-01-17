#pragma once

namespace Deltares {
    namespace ProbLibCore {

        enum class EnumDistributions
        {
            deterministic,
            uniform,
            normal,
            lognormal,
            lognormal2,
            shiftedExponential,
            gumbel,
            gumbel2,
            weibull,
            rayleigh,
            pareto,
            triangular,
            table,
            tableT,
            conditionalWeibull,
            modifiedGumbel,
            truncatedModifiedGumbel,
            RayleighN = 18,
            truncatedNormal,
            beta,
            uspace
        };
    }
}
