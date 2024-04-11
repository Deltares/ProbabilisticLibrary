#pragma once

#include "../../Deltares.Probabilistic/Statistics/DistributionType.h"
#include "../../Deltares.Probabilistic/Statistics/DistributionPropertyType.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			public enum class DistributionType { Deterministic, Normal, LogNormal, Uniform, Triangular, Trapezoidal, Exponential, Gamma, Beta, Frechet, Weibull, Gumbel, GeneralizedExtremeValue, StudentT, Rayleigh, RayleighN, Pareto, GeneralizedPareto, Table, CDFCurve, Discrete, Bernoulli, Poisson, Composite, X, Qualitative };
			public enum class DistributionPropertyType { Location, Scale, Minimum, Maximum, Shift, ShiftB, Shape, ShapeB, Observations };

			public ref class DistributionTypeConverter
			{
			public:
				static Statistics::DistributionType getNativeDistributionType(DistributionType distributionType);
				static DistributionType getManagedDistributionType(Statistics::DistributionType distributionType);
				static Statistics::DistributionPropertyType getNativeDistributionPropertyType(Wrappers::DistributionPropertyType distributionPropertyType);
			};
		}
	}
}


