#pragma once

#include "../../Deltares.Probabilistic.Kernels/Model/ReliabilityResult.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class ReliabilityResultWrapper
			{
			private:
				ReliabilityResult* m_report;

			public:
				ReliabilityResultWrapper()
				{
					m_report = new ReliabilityResult();
				}
				ReliabilityResultWrapper(ReliabilityResult* reliabilityResult)
				{
					m_report = reliabilityResult;
				}
				~ReliabilityResultWrapper() { this->!ReliabilityResultWrapper(); }
				!ReliabilityResultWrapper() { delete m_report; }

				property int ProxyIteration
				{
					int get() { return m_report->ProxyIteration; }
					void set(int value) { m_report->ProxyIteration = value; }
				}

				property int Run
				{
					int get() { return m_report->Run; }
					void set(int value) { m_report->Run = value; }
				}

				property int Index
				{
					int get() { return m_report->Index; }
					void set(int value) { m_report->Index = value; }
				}

				property double Reliability
				{
					double get() { return m_report->Reliability; }
					void set(double value) { m_report->Reliability = value; }
				}

				property double LowReliability
				{
					double get() { return m_report->LowReliability; }
					void set(double value) { m_report->LowReliability = value; }
				}

				property double HighReliability
				{
					double get() { return m_report->HighReliability; }
					void set(double value) { m_report->HighReliability = value; }
				}

				property double Samples
				{
					double get() { return m_report->Samples; }
					void set(double value) { m_report->Samples = value; }
				}

				property double ConvBeta
				{
					double get() { return m_report->ConvBeta; }
					void set(double value) { m_report->ConvBeta = value; }
				}

				property double ProxyConvergence
				{
					double get() { return m_report->ProxyConvergence; }
					void set(double value) { m_report->ProxyConvergence = value; }
				}

				property double Variation
				{
					double get() { return m_report->Variation; }
					void set(double value) { m_report->Variation = value; }
				}

				property double Difference
				{
					double get() { return m_report->Difference; }
					void set(double value) { m_report->Difference = value; }
				}

				property double Contribution
				{
					double get() { return m_report->Contribution; }
					void set(double value) { m_report->Contribution = value; }
				}

				void SetSettings(ReliabilityResult* reliabilityResult)
				{
					m_report = reliabilityResult;
				}
			};
		}
	}
}


