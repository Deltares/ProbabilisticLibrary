#pragma once

#include <memory>

#include "../../Deltares.Probabilistic.Kernels/Reliability/ConvergenceReport.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class ConvergenceReportWrapper
			{
			public:
				ConvergenceReportWrapper() {}
				ConvergenceReportWrapper(std::shared_ptr<Reliability::ConvergenceReport> report)
				{
					this->Convergence = report->Convergence;
					this->FailedSamples = report->FailedSamples;
					this->FailFraction = report->FailFraction;
					this->RelaxationFactor = report->RelaxationFactor;
					this->VarianceFactor = report->VarianceFactor;
					this->MaxWeight = report->MaxWeight;
					this->FailWeight = report->FailWeight;
					this->FailedSamples = report->FailedSamples;
					this->IsConverged = report->IsConverged;
				}

				property double Convergence;
				property int FailedSamples;
				property double FailFraction;
				property double RelaxationFactor;
				property double VarianceFactor;
				property double MaxWeight;
				property double FailWeight;
				property bool IsConverged;
			};
		}
	}
}


