#include "StochastSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        bool StochastSettings::isMinMaxDefault()
        {
            return this->MinValue <= -Statistics::StandardNormal::UMax && this->MaxValue >= Statistics::StandardNormal::UMax;
        }

        void StochastSettings::initializeForRun()
        {
            this->XMinValue = Statistics::StandardNormal::getPFromU(this->MinValue);
            this->XMaxValue = Statistics::StandardNormal::getPFromU(this->MaxValue);
        }

        double StochastSettings::getRepresentativeU(double u)
        {
            return this->stochast->getRepresentativeU(u);
        }

        std::shared_ptr<StochastSettings> StochastSettings::clone()
        {
            std::shared_ptr<StochastSettings> clone = std::make_shared<StochastSettings>();

            clone->IsInitializationAllowed = this->IsInitializationAllowed;
            clone->IsQualitative = this->IsQualitative;
            clone->IsVarianceAllowed = this->IsVarianceAllowed;
            clone->Intervals = this->Intervals;
            clone->MaxValue = this->MaxValue;
            clone->MinValue = this->MinValue;
            clone->XMaxValue = this->XMaxValue;
            clone->XMinValue = this->XMinValue;
            clone->StartValue = this->StartValue;
            clone->UncorrelatedStartValue = this->UncorrelatedStartValue;
            clone->VarianceFactor = this->VarianceFactor;
            clone->StochastIndex = this->StochastIndex;
            clone->stochast = this->stochast;

            return clone;

        }

    }
}

