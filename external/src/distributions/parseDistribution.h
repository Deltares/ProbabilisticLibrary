#pragma once
#include <vector>
#include <string>
#include "distrib.h"

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

        class parseDistribution
        {
        public:
            parseDistribution() {};
            EnumDistributions parse(const std::string& s) const;
            distrib* parse(const std::vector<std::string>& s, const std::vector<double>& p4) const;
            distrib* parse(const EnumDistributions distType, const std::string& name,
                const std::vector<double>& p4) const;
            size_t activeParameters(const EnumDistributions distType) const;
        };
    }
}
