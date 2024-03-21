#include "HohenbichlerZ.h"
#include "../Statistics/StandardNormal.h"

using namespace Deltares::Statistics;

namespace Deltares {
    namespace Reliability {

        HohenbichlerZ::HohenbichlerZ(const double betaV, const double pfU, const double r) :
            beta2(betaV), pf1(pfU), rho(r), squaredRoot(sqrt(1.0 - r * r)) {}

        //> ZHohenbichler: Z-function for Hohenbichler with input parameters x and output parameter z
        double HohenbichlerZ::zfunc(std::vector<double>& x) const
        {
            //
            // u     : u-value (standard normal distributed)
            // w     : w-value (standard normal distributed)
            // beta2 : Reliability index for stochastic variable 2 (smallest reliability index)
            // pf1   : Probability of failure for stochastic variable 1 (smallest probability of failure)
            // rho   : Correlation coefficient between Z_1 and Z_2
            //
            const double u = x[0];
            const double w = x[1];
            double phiU = StandardNormal::getPFromU(u);
            double uAccent = StandardNormal::getUFromQ(pf1 * phiU);
            return beta2 - rho * uAccent - squaredRoot * w;
        }

        void HohenbichlerZ::FDelegate(std::shared_ptr<Deltares::Models::ModelSample> s)
        {
            auto x = std::vector<double>(2);
            x[0] = s->Values[0];
            x[1] = s->Values[1];
            auto z = zfunc(x);
            s->Z = z;
        }
    }
}
