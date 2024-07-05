#pragma once

namespace Deltares
{
    namespace Statistics
    {
        /**
         * \brief Contains corresponding exceeding and non-exceeding probability
         */
        struct PQ
        {
            /**
             * \brief Non-exceeding probability
             */
            double p;

            /**
             * \brief Exceeding probability
             */
            double q;
        };

        /**
         * \brief Provides conversions between probabilities (p,q), reliability (u) and return period (r)
         */
        class StandardNormal
        {
        private:
            static void normp(const double z, double& p, double& q, double& pdf);
            static constexpr double qMin = 1.0e-300;

        public:
            static double getPFromU(const double u);
            static double getQFromU(const double u);
            static double getUFromP(const double p);
            static double getUFromQ(const double q);
            static double getUFromPQ(const PQ* pq);
            static double getRFromP(const double p);
            static double getPFromR(const double r);
            static double getRFromU(double u);
            static double getUFromR(double r);
            static void getPQfromU(const double u, double& p, double& q);
            static PQ getPQFromU(const double u);
            static double ReturnTimeFromBeta(const double beta);
            static double FreqFromBeta(const double beta);
            static double LogQFromBeta(const double beta);

            /**
             * \brief Maximum allowed beta value, due to numerical limitations
             */
            static constexpr double BetaMax = 40;

            /**
             * \brief Maximum allowed reliability value, due to numerical limitations
             */
            static constexpr double UMax = 8;
        };
    }
}

