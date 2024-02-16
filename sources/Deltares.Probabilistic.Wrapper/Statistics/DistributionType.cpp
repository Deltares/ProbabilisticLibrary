#include "DistributionType.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			Statistics::DistributionType DistributionTypeConverter::getNativeDistributionType(Wrappers::DistributionType distributionType)
			{
				switch (distributionType)
				{
				case Wrappers::DistributionType::Deterministic: return Statistics::DistributionType::Deterministic;
				case Wrappers::DistributionType::Normal: return Statistics::DistributionType::Normal;
				case Wrappers::DistributionType::LogNormal: return Statistics::DistributionType::LogNormal;
				case Wrappers::DistributionType::Uniform: return Statistics::DistributionType::Uniform;
				case Wrappers::DistributionType::Gumbel: return Statistics::DistributionType::Gumbel;
				case Wrappers::DistributionType::Discrete: return Statistics::DistributionType::Discrete;
				case Wrappers::DistributionType::Qualitative: return Statistics::DistributionType::Qualitative;
				default: throw gcnew System::NotSupportedException(distributionType.ToString());
				}
			}

			DistributionType DistributionTypeConverter::getManagedDistributionType(Statistics::DistributionType distributionType)
			{
				switch (distributionType)
				{
				case Statistics::DistributionType::Deterministic: return Wrappers::DistributionType::Deterministic;
				case Statistics::DistributionType::Normal: return Wrappers::DistributionType::Normal;
				case Statistics::DistributionType::LogNormal: return Wrappers::DistributionType::LogNormal;
				case Statistics::DistributionType::Uniform: return Wrappers::DistributionType::Uniform;
				case Statistics::DistributionType::Gumbel: return Wrappers::DistributionType::Gumbel;
				case Statistics::DistributionType::Discrete: return Wrappers::DistributionType::Discrete;
				default: throw gcnew System::NotSupportedException("distribution type");
				}
			}
		}
	}
}


