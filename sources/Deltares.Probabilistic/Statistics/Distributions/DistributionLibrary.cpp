#include "DistributionLibrary.h"

#include "DeterministicDistribution.h"
#include "FrechetDistribution.h"
#include "GumbelDistribution.h"
#include "InvertedDistribution.h"
#include "LogNormalDistribution.h"
#include "NormalDistribution.h"
#include "QualitativeDistribution.h"
#include "RayleighDistribution.h"
#include "RayleighNDistribution.h"
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
			case DistributionType::Gumbel: distribution = std::make_shared<GumbelDistribution>(); break;
			case DistributionType::Weibull: distribution = std::make_shared<WeibullDistribution>(); break;
			case DistributionType::Frechet: distribution = std::make_shared<FrechetDistribution>(); break;
			case DistributionType::Rayleigh: distribution = std::make_shared<RayleighDistribution>(); break;
			case DistributionType::RayleighN: distribution = std::make_shared<RayleighNDistribution>(); break;
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

