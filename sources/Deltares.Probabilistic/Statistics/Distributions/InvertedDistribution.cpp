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
#include "InvertedDistribution.h"
#include "../Stochast.h"
#include "../../Math/NumericSupport.h"

#include <algorithm>

namespace Deltares
{
    namespace Statistics
    {
        double InvertedDistribution::getInvertedValue(const StochastProperties& stochast, double value) const
        {
            double center = isShiftUsed() ? stochast.Shift : 0.0;
            return 2.0 * center - value;
        }

        StochastProperties InvertedDistribution::getInvertedStochast(const StochastProperties& stochast) const
        {
            auto invertedStochast = StochastProperties();
            this->copyFromInverted(invertedStochast, stochast);
            return invertedStochast;
        }

        void InvertedDistribution::copyFromInverted(StochastProperties& target, const StochastProperties& source) const
        {
            target.Location = source.Location;
            target.Scale = source.Scale;
            target.Shape = source.Shape;
            target.ShapeB = source.ShapeB;
            target.Shift = source.Shift;
            target.ShiftB = source.ShiftB;
            target.Observations = source.Observations;

            target.Minimum = getInvertedValue(source, source.Maximum);
            target.Maximum = getInvertedValue(source, source.Minimum);
        }

        void InvertedDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            const double invertedMean = this->getInvertedValue(stochast, mean);
            auto invertedStochast = getInvertedStochast(stochast);

            this->innerDistribution->setMeanAndDeviation(invertedStochast, invertedMean, deviation);

            copyFromInverted(stochast, invertedStochast);
        }

        void InvertedDistribution::setShift(StochastProperties& stochast, const double shift, bool inverted)
        {
            this->innerDistribution->setShift(stochast, shift, true);
        }

        bool InvertedDistribution::isVarying(StochastProperties& stochast)
        {
            auto invertedStochast = getInvertedStochast(stochast);

            return this->innerDistribution->isVarying(invertedStochast);
        }

        double InvertedDistribution::getMean(StochastProperties& stochast)
        {
            auto invertedStochast = getInvertedStochast(stochast);
            return this->getInvertedValue(stochast, this->innerDistribution->getMean(invertedStochast));
        }

        double InvertedDistribution::getDeviation(StochastProperties& stochast)
        {
            auto invertedStochast = getInvertedStochast(stochast);
            return this->innerDistribution->getDeviation(invertedStochast);
        }

        double InvertedDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            auto invertedStochast = getInvertedStochast(stochast);
            const double xInvert = this->innerDistribution->getXFromU(invertedStochast, -u);

            return this->getInvertedValue(stochast, xInvert);
        }

        double InvertedDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            const double xInvert = this->getInvertedValue(stochast, x);

            auto invertedStochast = getInvertedStochast(stochast);
            return - this->innerDistribution->getUFromX(invertedStochast, xInvert);
        }

        double InvertedDistribution::getPDF(StochastProperties& stochast, double x)
        {
            const double xInvert = this->getInvertedValue(stochast, x);

            auto invertedStochast = getInvertedStochast(stochast);
            return this->innerDistribution->getPDF(invertedStochast, xInvert);
        }

        double InvertedDistribution::getCDF(StochastProperties& stochast, double x)
        {
            const double xInvert = getInvertedValue(stochast, x);

            auto invertedStochast = getInvertedStochast(stochast);
            return 1.0 - innerDistribution->getCDF(invertedStochast, xInvert);
        }

        void InvertedDistribution::setXAtU(StochastProperties& stochast, double x, double u, ConstantParameterType constantType)
        {
            double xInvert = this->getInvertedValue(stochast, x);

            auto invertedStochast = getInvertedStochast(stochast);
            this->innerDistribution->setXAtU(invertedStochast, xInvert, -u, constantType);

            copyFromInverted(stochast, invertedStochast);
        }

        void InvertedDistribution::fit(StochastProperties& stochast, const std::vector<double>& values, const double shift)
        {
            // fit the shift first
            // do not use inverted value, because it depends on stochast->Shift, which is not known yet (because it has to be fitted)
            if (isShiftUsed())
            {
                std::vector<double> zeroInvertedValues = Numeric::NumericSupport::select(values, [](double x) {return -x; });

                auto invertedStochast = getInvertedStochast(stochast);
                this->innerDistribution->fit(invertedStochast, zeroInvertedValues, shift);

                stochast.Shift = -stochast.Shift;
            }

            std::vector<double> invertedValues = Numeric::NumericSupport::select(values, [this, &stochast](double x)
                {return this->getInvertedValue(stochast, x); });

            auto invertedStochast = getInvertedStochast(stochast);
            this->innerDistribution->fit(invertedStochast, invertedValues, -shift);

            copyFromInverted(stochast, invertedStochast);
        }

        void InvertedDistribution::fitPrior(StochastProperties& stochast, const std::vector<double>& values, StochastProperties& prior, const double shift)
        {
            // fit the shift first
            // do not use inverted value, because it depends on stochast->Shift, which is not known yet (because it has to be fitted)
            if (isShiftUsed())
            {
                std::vector<double> zeroInvertedValues = Numeric::NumericSupport::select(values, [](double x) {return -x; });

                auto invertedStochast = getInvertedStochast(stochast);
                auto invertedPrior = getInvertedStochast(prior);

                this->innerDistribution->fitPrior(invertedStochast, zeroInvertedValues, invertedPrior, shift);

                stochast.Shift = -stochast.Shift;
            }

            std::vector<double> invertedValues = Numeric::NumericSupport::select(values, [this, &stochast](double x)
            {
                return this->getInvertedValue(stochast, x);
            });

            auto invertedStochast = getInvertedStochast(stochast);
            auto invertedPrior = getInvertedStochast(prior);

            this->innerDistribution->fitPrior(invertedStochast, invertedValues, invertedPrior, -shift);

            copyFromInverted(stochast, invertedStochast);
        }

        double InvertedDistribution::getMaxShiftValue(std::vector<double>& values)
        {
            std::vector<double> zeroInvertedValues = Numeric::NumericSupport::select(values, [](double x) {return -x; });

            return - innerDistribution->getMaxShiftValue(zeroInvertedValues);
        }

        void InvertedDistribution::validate(Logging::ValidationReport& report, StochastProperties& stochast, std::string& subject)
        {
            auto invertedStochast = getInvertedStochast(stochast);
            this->innerDistribution->validate(report, invertedStochast, subject);
        }

        bool InvertedDistribution::isShiftUsed() const
        {
            return std::ranges::any_of(this->innerDistribution->getParameters(), [](DistributionPropertyType parameter)
            {
                return parameter == DistributionPropertyType::Shift;
            });
        }

        double InvertedDistribution::getLogLikelihood(StochastProperties& stochast, double x)
        {
            const double xInvert = this->getInvertedValue(stochast, x);

            auto invertedStochast = getInvertedStochast(stochast);
            return this->innerDistribution->getLogLikelihood(invertedStochast, xInvert);
        }

        std::vector<double> InvertedDistribution::getSpecialPoints(StochastProperties& stochast)
        {
            auto invertedStochast = getInvertedStochast(stochast);
            std::vector<double> specialPoints = this->innerDistribution->getSpecialPoints(invertedStochast);

            return Numeric::NumericSupport::select(specialPoints, [this, &stochast](double x)
                {return this->getInvertedValue(stochast, x); });
        }

        std::vector<DistributionPropertyType> InvertedDistribution::getParameters()
        {
            return innerDistribution->getParameters();
        }
    }
}

