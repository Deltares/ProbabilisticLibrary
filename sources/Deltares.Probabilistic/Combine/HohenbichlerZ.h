#pragma once
#include "../Model/ZModel.h"

namespace Deltares {
    namespace Reliability {

        class HohenbichlerZ
        {
        public:
            HohenbichlerZ(const double betaV, const double pfU, const double r);
            double zfunc(std::vector<double>& x) const;
            void FDelegate(std::shared_ptr<Deltares::Models::ModelSample> s);
        private:
            const double beta2;
            const double pf1;
            const double rho;
            const double squaredRoot;
        };
    }
}
