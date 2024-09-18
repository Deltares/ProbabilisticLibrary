// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
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
            static double getUFromPQ(const PQ& pq);
            static double getRFromP(const double p);
            static double getPFromR(const double r);
            static double getRFromU(double u);
            static double getUFromR(double r);
            static void getPQfromU(const double u, double& p, double& q);
            static PQ getPQFromU(const double u);
            static double ReturnTimeFromBeta(const double beta);
            static double BetaFromReturnTime(const double return_time);
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

