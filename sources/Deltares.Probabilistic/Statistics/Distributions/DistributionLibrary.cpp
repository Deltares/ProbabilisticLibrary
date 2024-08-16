#include "DistributionLibrary.h"

#include "BetaDistribution.h"
#include "ConditionalWeibullDistribution.h"
#include "DeterministicDistribution.h"
#include "FragilityCurveDistribution.h"
#include "FrechetDistribution.h"
#include "GeneralizedExtremeValueDistribution.h"
#include "GumbelDistribution.h"
#include "HistogramDistribution.h"
#include "InvertedDistribution.h"
#include "LogNormalDistribution.h"
#include "NormalDistribution.h"
#include "DiscreteDistribution.h"
#include "ExponentialDistribution.h"
#include "GeneralizedParetoDistribution.h"
#include "QualitativeDistribution.h"
#include "RayleighDistribution.h"
#include "RayleighNDistribution.h"
#include "ParetoDistribution.h"
#include "TriangularDistribution.h"
#include "TrapezoidalDistribution.h"
#include "TruncatedDistribution.h"
#include "UniformDistribution.h"
#include "WeibullDistribution.h"

#include "../../Utils/probLibException.h"

namespace Deltares
{
    namespace Statistics
    {
        std::shared_ptr<Statistics::Distribution> DistributionLibrary::getDistribution(DistributionType distributionType, bool truncated, bool inverted)
        {
            std::shared_ptr<Statistics::Distribution> distribution = nullptr;

            switch (distributionType)
            {
            case DistributionType::Deterministic: distribution = std::make_shared<DeterministicDistribution>(); break;
            case DistributionType::Normal: distribution = std::make_shared<NormalDistribution>(); break;
            case DistributionType::LogNormal: distribution = std::make_shared<LogNormalDistribution>(); break;
            case DistributionType::Uniform: distribution = std::make_shared<UniformDistribution>(); break;
            case DistributionType::Triangular: distribution = std::make_shared<TriangularDistribution>(); break;
            case DistributionType::Trapezoidal: distribution = std::make_shared<TrapezoidalDistribution>(); break;
            case DistributionType::Exponential: distribution = std::make_shared<ExponentialDistribution>(); break;
            case DistributionType::Gumbel: distribution = std::make_shared<GumbelDistribution>(); break;
            case DistributionType::Weibull: distribution = std::make_shared<WeibullDistribution>(); break;
            case DistributionType::ConditionalWeibull: distribution = std::make_shared<ConditionalWeibullDistribution>(); break;
            case DistributionType::Frechet: distribution = std::make_shared<FrechetDistribution>(); break;
            case DistributionType::GeneralizedExtremeValue: distribution = std::make_shared<GeneralizedExtremeValueDistribution>(); break;
            case DistributionType::Rayleigh: distribution = std::make_shared<RayleighDistribution>(); break;
            case DistributionType::RayleighN: distribution = std::make_shared<RayleighNDistribution>(); break;
            case DistributionType::Pareto: distribution = std::make_shared<ParetoDistribution>(); break;
            case DistributionType::GeneralizedPareto: distribution = std::make_shared<GeneralizedParetoDistribution>(); break;
            case DistributionType::Beta: distribution = std::make_shared<BetaDistribution>(); break;
            case DistributionType::Table: distribution = std::make_shared<HistogramDistribution>(); break;
            case DistributionType::CDFCurve: distribution = std::make_shared<FragilityCurveDistribution>(); break;
            case DistributionType::Discrete: distribution = std::make_shared<DiscreteDistribution>(); break;
            case DistributionType::Qualitative: distribution = std::make_shared<QualitativeDistribution>(); break;
            default:
                throw Deltares::Reliability::probLibException("Distribution type not supported");
            }

            if (truncated && distribution->canTruncate())
            {
                distribution = std::make_shared<TruncatedDistribution>(distribution);
            }

            if (inverted && distribution->canInvert())
            {
                distribution = std::make_shared<InvertedDistribution>(distribution);
            }

            return distribution;
        }
    }
}

