#pragma once
#include <vector>

namespace Deltares {
    namespace ProbLibCore {

        class dgammaDs
        {
        public:
            dgammaDs(const double a);
            double Dgammq(const double a, const double x);
            double DGamLn(const double x);
        private:
            void DGser(double& gamser, const double a, const double x);
            void DGcf(double& gammcf, const double a, const double x);
            double prepareA = -999.0;
            double prepareLog = -999.0;
            const std::vector<double> cof =
            { 76.18009172947146, -86.50532032941677, 24.01409824083091,
             -1.231739572450155,  .1208650973866179e-2,  -.5395239384953e-5 };
        };
    }
}
