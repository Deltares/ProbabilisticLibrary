#pragma once

#include <cmath>

namespace Deltares
{
    namespace Statistics
    {
        /**
          * \brief Base class of a stochast
          * \remark Introduced to resolve a circular reference from stochast->ContributingStochasts->Stochast
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

            virtual bool isValid() { return false; }

            virtual bool isVarying() { return false; }

        };
    }
}

