#pragma once

#include "../../Deltares.Probabilistic/Statistics/DistributionType.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			public enum class DistributionType { Deterministic, Normal, LogNormal, Uniform, Triangular, Trapezoidal, Exponential, Gamma, Beta, Frechet, Weibull, Gumbel, GeneralizedExtremeValue, StudentT, Rayleigh, Pareto, GeneralizedPareto, Table, CDFCurve, Discrete, Bernoulli, Poisson, Composite, X, Qualitative };

			public ref class DistributionTypeConverter
			{
			public:
				static Statistics::DistributionType getNativeDistributionType(DistributionType distributionType);
				static DistributionType getManagedDistributionType(Statistics::DistributionType distributionType);
			};
		}
	}
}


