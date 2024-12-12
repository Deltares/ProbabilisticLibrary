// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include <cmath>
#include <vector>

namespace Deltares
{
    namespace Statistics
    {
        /**
          * \brief Base class of a stochast
          * \remark Introduced to prevent a circular reference from stochast->ContributingStochasts->Stochast
          */
        class BaseStochast
        {
        public:
            /**
             * \brief Gets the Cumulative Density Function (CDF) for a given x-value
             * \param x Given x-value
             * \return CDF
             */
            virtual double getCDF(double x) { return std::nan(""); }

            /**
             * \brief Gets the Probability Density Function (PDF) for a given x-value
             * \param x Given x-value
             * \return PDF
             */
            virtual double getPDF(double x) { return std::nan(""); }

            /**
             * \brief Gets the x-value corresponding to a given u-value
             * \param u Given u-value
             * \return x-value
             */
            virtual double getXFromU(double u) { return std::nan(""); }

            /**
             * \brief Gets the u-value corresponding to a given x-value
             * \param x Given x-value
             * \return u-value
             */
            virtual double getUFromX(double x) { return std::nan(""); }

            /**
             * \brief Gets the mean value of a stochast
             * \return Mean value
             */
            virtual double getMean() { return std::nan(""); }

            /**
             * \brief Gets a number of interesting x-values
             * \remark This method is used when plotting PDF and CDF
             * \return Interesting x-values
             */
            virtual std::vector<double> getSpecialXValues() { return std::vector<double>(); }

            virtual bool isValid() { return false; }

            virtual bool isVarying() { return false; }

        };
    }
}

