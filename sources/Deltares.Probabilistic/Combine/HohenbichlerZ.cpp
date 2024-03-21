#include "HohenbichlerZ.h"
#include "../Statistics/StandardNormal.h"

using namespace Deltares::Statistics;

namespace Deltares {
    namespace Reliability {

        HohenbichlerZ::HohenbichlerZ(const double betaV, const double pfU, const double r) :
            beta2(betaV), pf1(pfU), rho(r), squaredRoot(sqrt(1.0 - r * r)) {}

        //> ZHohenbichler: Z-function for Hohenbichler with input parameters x and output parameter z
        void HohenbichlerZ::zfunc(std::shared_ptr<Deltares::Models::ModelSample> s) const
        {
            //
            // u     : u-value (standard normal distributed)
            // w     : w-value (standard normal distributed)
            // beta2 : Reliability index for stochastic variable 2 (smallest reliability index)
            // pf1   : Probability of failure for stochastic variable 1 (smallest probability of failure)
            // rho   : Correlation coefficient between Z_1 and Z_2
            //
            const double u = s->Values[0];
            const double w = s->Values[1];
            double phiU = StandardNormal::getPFromU(u);
            double uAccent = StandardNormal::getUFromQ(pf1 * phiU);
            auto z = beta2 - rho * uAccent - squaredRoot * w;
            s->Z = z;
        }
    }
}
