#include "Stochast.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			Statistics::DistributionType Stochast::getNativeDistributionType(WrapperDistributionType distributionType)
			{
				switch (distributionType)
				{
				case WrapperDistributionType::Deterministic: return Statistics::DistributionType::Deterministic;
				case WrapperDistributionType::Normal: return Statistics::DistributionType::Normal;
				case WrapperDistributionType::LogNormal: return Statistics::DistributionType::LogNormal;
				case WrapperDistributionType::Uniform: return Statistics::DistributionType::Uniform;
				case WrapperDistributionType::Gumbel: return Statistics::DistributionType::Gumbel;
				case WrapperDistributionType::Discrete: return Statistics::DistributionType::Discrete;
				case WrapperDistributionType::Qualitative: return Statistics::DistributionType::Qualitative;
				default: throw gcnew System::NotSupportedException(distributionType.ToString());
				}
			}

			WrapperDistributionType Stochast::getManagedDistributionType(Statistics::DistributionType distributionType)
			{
				switch (distributionType)
				{
				case Statistics::DistributionType::Deterministic: return WrapperDistributionType::Deterministic;
				case Statistics::DistributionType::Normal: return WrapperDistributionType::Normal;
				case Statistics::DistributionType::LogNormal: return WrapperDistributionType::LogNormal;
				case Statistics::DistributionType::Uniform: return WrapperDistributionType::Uniform;
				case Statistics::DistributionType::Gumbel: return WrapperDistributionType::Gumbel;
				case Statistics::DistributionType::Discrete: return WrapperDistributionType::Discrete;
				default: throw gcnew System::NotSupportedException("distribution type");
				}
			}

			void Stochast::updateStochast()
			{
				m_stochast->DiscreteValues.clear();
				for (size_t i = 0; i < this->discreteValues->Count; i++)
				{
					m_stochast->DiscreteValues.push_back(discreteValues[i]->GetValue());
				}

				m_stochast->HistogramValues.clear();
				for (size_t i = 0; i < this->histogramValues->Count; i++)
				{
					m_stochast->HistogramValues.push_back(histogramValues[i]->GetValue());
				}

				m_stochast->FragilityValues.clear();
				for (size_t i = 0; i < this->fragilityValues->Count; i++)
				{
					m_stochast->FragilityValues.push_back(fragilityValues[i]->GetValue());
				}

				m_stochast->ValueSet->StochastValues.clear();
				if (this->IsVariableStochast)
				{
					for (size_t i = 0; i < this->ValueSet->StochastValues->Count; i++)
					{
						m_stochast->ValueSet->StochastValues.push_back(this->ValueSet->StochastValues[i]->GetValue());
					}
				}
			}
		}
	}
}
