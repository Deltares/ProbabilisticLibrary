#pragma once
#include "../Statistics/Stochast.h"
#include "../Statistics/StandardNormal.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public enum class WrapperDistributionType {Deterministic, Normal, LogNormal, Uniform, Gumbel};


			public ref class StochastWrapper
			{
			private:
				Statistics::Stochast* m_stochast;

				Statistics::DistributionType getDistributionType(WrapperDistributionType distributionType)
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

				WrapperDistributionType getDistributionType(Statistics::DistributionType distributionType)
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

			public:
				StochastWrapper() { m_stochast = new Statistics::Stochast(); }
				StochastWrapper(WrapperDistributionType distributionType, double* values)
				{
					const Statistics::DistributionType nativeDistributionType = getDistributionType(distributionType);
					m_stochast = new Statistics::Stochast(nativeDistributionType, values);
				}
				~StochastWrapper() { this->!StochastWrapper(); }
				!StochastWrapper() { delete m_stochast; }

				Statistics::Stochast* GetStochast()
				{
					return m_stochast;
				}

				property WrapperDistributionType DistributionType
				{
					WrapperDistributionType get() { return getDistributionType(m_stochast->getDistributionType()); }
					void set(WrapperDistributionType value) { m_stochast->setDistributionType(getDistributionType(value)); }
				}

				property double Mean
				{
					double get() { return m_stochast->getMean(); }
					void set(double value) { m_stochast->setMean(value); }
				}

				property double Deviation
				{
					double get() { return m_stochast->getDeviation(); }
					void set(double value) { m_stochast->setDeviation(value); }
				}

				property double Location
				{
					double get() { return m_stochast->Location; }
					void set(double value) { m_stochast->Location = value; }
				}

				property double Scale
				{
					double get() { return m_stochast->Scale; }
					void set(double value) { m_stochast->Scale = value; }
				}

				property double Shift
				{
					double get() { return m_stochast->Shift; }
					void set(double value) { m_stochast->Shift = value; }
				}

				property double ShiftB
				{
					double get() { return m_stochast->ShiftB; }
					void set(double value) { m_stochast->ShiftB = value; }
				}

				property double Shape
				{
					double get() { return m_stochast->Shape; }
					void set(double value) { m_stochast->Shape = value; }
				}

				property double ShapeB
				{
					double get() { return m_stochast->ShapeB; }
					void set(double value) { m_stochast->ShapeB = value; }
				}

				property double Minimum
				{
					double get() { return m_stochast->Minimum; }
					void set(double value) { m_stochast->Minimum = value; }
				}

				property double Maximum
				{
					double get() { return m_stochast->Maximum; }
					void set(double value) { m_stochast->Maximum = value; }
				}

				property int Observations
				{
					int get() { return m_stochast->Observations; }
					void set(int value) { m_stochast->Observations = value; }
				}

				property bool Truncated
				{
					bool get() { return m_stochast->isTruncated(); }
					void set(bool value) { m_stochast->setTruncated(value); }
				}

				property bool Inverted
				{
					bool get() { return m_stochast->isInverted(); }
					void set(bool value) { m_stochast->setInverted(value); }
				}

				double GetXFromU(double u)
				{
					return m_stochast->getXFromU(u);
				}

				double GetXFromP(double p)
				{
					double u = StandardNormal::getUFromP(p);
					return m_stochast->getXFromU(u);
				}

				double GetUFromX(double x)
				{
					return m_stochast->getUFromX(x);
				}
			};
		}
	}
}

