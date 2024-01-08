#pragma once
#include "../Statistics/Stochast.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class StochastWrapper
			{
			private:
				Stochast* m_stochast;

			public:
				StochastWrapper() { m_stochast = new Stochast(); }
				StochastWrapper(int distributionType, double* values)
				{
					m_stochast = new Stochast((DistributionType) distributionType, values);
				}
				~StochastWrapper() { this->!StochastWrapper(); }
				!StochastWrapper() { delete m_stochast; }

				Stochast* GetStochast()
				{
					return m_stochast;
				}

				property int Distribution
				{
					int get() { return m_stochast->getDistributionType(); }
					void set(int value) { m_stochast->setDistributionType((DistributionType)value); }
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
			};
		}
	}
}

