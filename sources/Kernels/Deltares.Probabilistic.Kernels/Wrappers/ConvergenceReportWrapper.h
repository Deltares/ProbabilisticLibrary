#pragma once

#include "RandomSettingsWrapper.h"
#include "RunSettingsWrapper.h"
#include "../Model/ConvergenceReport.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class ConvergenceReportWrapper
			{
			private:
				ConvergenceReport* m_report;

			public:
				ConvergenceReportWrapper()
				{
					m_report = new ConvergenceReport();
				}
				ConvergenceReportWrapper(ConvergenceReport* convergenceReport)
				{
					m_report = convergenceReport;
				}
				~ConvergenceReportWrapper() { this->!ConvergenceReportWrapper(); }
				!ConvergenceReportWrapper() { delete m_report; }

				property double Convergence
				{
					double get() { return m_report->Convergence; }
					void set(double value) { m_report->Convergence = value; }
				}

				property int FailedSamples
				{
					int get() { return m_report->FailedSamples; }
					void set(int value) { m_report->FailedSamples = value; }
				}

				property double FailFraction
				{
					double get() { return m_report->FailFraction; }
					void set(double value) { m_report->FailFraction = value; }
				}

				property double RelaxationFactor
				{
					double get() { return m_report->RelaxationFactor; }
					void set(double value) { m_report->RelaxationFactor = value; }
				}

				property double VarianceFactor
				{
					double get() { return m_report->VarianceFactor; }
					void set(double value) { m_report->VarianceFactor = value; }
				}

				property double MaxWeight
				{
					double get() { return m_report->MaxWeight; }
					void set(double value) { m_report->MaxWeight = value; }
				}

				property double FailWeight
				{
					double get() { return m_report->FailWeight; }
					void set(double value) { m_report->FailWeight = value; }
				}

				void SetSettings(ConvergenceReport* convergenceReport)
				{
					m_report = convergenceReport;
				}
			};
		}
	}
}


