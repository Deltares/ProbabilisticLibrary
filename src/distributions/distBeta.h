#pragma once
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        class distBeta : public distrib
        {
        public:
            distBeta(const double p1, const double p2, const double p3, const double p4);
            distBeta(const std::string& s, const double p1, const double p2, const double p3, const double p4);
            virtual double getValue(const double u) const;
            double distributionBetaInvPQ(const double p, const double q) const;
        private:
            void transform();
            const double a;
            const double b;
            const double alpha;
            const double beta;
            double lnComplBetaFunc;
        };

        class distBeta4p : public distrib4p
        {
        public:
            virtual double getValue(const double u, const double p1, const double p2,
                const double p3, const double p4) const;
        };
    }
}
