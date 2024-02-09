#include "Stochast.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			Statistics::ConstantParameterType Stochast::getNativeConstantParameterType(Wrappers::ConstantParameterType constantParameterType)
			{
				switch (constantParameterType)
				{
				case Wrappers::ConstantParameterType::Deviation: return Statistics::ConstantParameterType::Deviation;
				case Wrappers::ConstantParameterType::VariationCoefficient: return Statistics::ConstantParameterType::VariationCoefficient;
				default: throw gcnew System::NotSupportedException(constantParameterType.ToString());
				}
			}

			Statistics::DistributionType Stochast::getNativeDistributionType(Wrappers::DistributionType distributionType)
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

			DistributionType Stochast::getManagedDistributionType(Statistics::DistributionType distributionType)
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
