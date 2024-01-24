#include "StochastWrapper.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			Statistics::DistributionType StochastWrapper::getNativeDistributionType(WrapperDistributionType distributionType)
			{
				switch (distributionType)
				{
				case WrapperDistributionType::Deterministic: return Statistics::DistributionType::Deterministic;
				case WrapperDistributionType::Normal: return Statistics::DistributionType::Normal;
				case WrapperDistributionType::LogNormal: return Statistics::DistributionType::LogNormal;
				case WrapperDistributionType::Uniform: return Statistics::DistributionType::Uniform;
				case WrapperDistributionType::Gumbel: return Statistics::DistributionType::Gumbel;
				default: throw gcnew System::NotSupportedException(distributionType.ToString());
				}
			}

			WrapperDistributionType StochastWrapper::getManagedDistributionType(Statistics::DistributionType distributionType)
			{
				switch (distributionType)
				{
				case Statistics::DistributionType::Deterministic: return WrapperDistributionType::Deterministic;
				case Statistics::DistributionType::Normal: return WrapperDistributionType::Normal;
				case Statistics::DistributionType::LogNormal: return WrapperDistributionType::LogNormal;
				case Statistics::DistributionType::Uniform: return WrapperDistributionType::Uniform;
				case Statistics::DistributionType::Gumbel: return WrapperDistributionType::Gumbel;
				default: throw gcnew System::NotSupportedException("distribution type");
				}
			}
		}
	}
}
