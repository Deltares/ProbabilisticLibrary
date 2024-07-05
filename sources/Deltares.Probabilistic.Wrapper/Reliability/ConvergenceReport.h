#pragma once

#include <memory>

#include "../../Deltares.Probabilistic/Reliability/ConvergenceReport.h"
#include "../Model/Sample.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class ConvergenceReport
            {
            public:
                ConvergenceReport() {}
                ConvergenceReport(std::shared_ptr<Reliability::ConvergenceReport> report)
                {
                    this->Convergence = report->Convergence;
                    this->FailedSamples = report->FailedSamples;
                    this->FailFraction = report->FailFraction;
                    this->RelaxationFactor = report->RelaxationFactor;
                    this->VarianceFactor = report->VarianceFactor;
                    this->MaxWeight = report->MaxWeight;
                    this->FailWeight = report->FailWeight;
                    this->FailedSamples = report->FailedSamples;
                    this->ZMargin = report->ZMargin;
                    this->IsConverged = report->IsConverged;
                }

                property double Convergence;
                property int FailedSamples;
                property double FailFraction;
                property double RelaxationFactor;
                property double VarianceFactor;
                property double MaxWeight;
                property double FailWeight;
                property double ZMargin;
                property bool IsConverged;

                Deltares::Models::Wrappers::Sample^ NearestStochastPoint = nullptr;
            };
        }
    }
}


