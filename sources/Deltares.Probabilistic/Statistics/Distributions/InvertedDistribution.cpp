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

namespace Deltares
{
    namespace Statistics
    {
        double InvertedDistribution::getInvertedValue(std::shared_ptr<StochastProperties> stochast, double value)
        {
            double center = this->innerDistribution->isShiftUsed() ? stochast->Shift : 0;
            return 2 * center - value;
        }

        std::shared_ptr<StochastProperties> InvertedDistribution::getInvertedStochast(std::shared_ptr<StochastProperties> stochast)
        {
            std::shared_ptr<StochastProperties> invertedStochast = std::make_shared<StochastProperties>();
            this->copyFromInverted(invertedStochast, stochast);
            return invertedStochast;
        }

        void InvertedDistribution::copyFromInverted(std::shared_ptr<StochastProperties> target, std::shared_ptr<StochastProperties> source)
        {
            target->Location = source->Location;
            target->Scale = source->Scale;
            target->Shape = source->Shape;
            target->ShapeB = source->ShapeB;
            target->Shift = source->Shift;
            target->ShiftB = source->ShiftB;
            target->Observations = source->Observations;

            target->Minimum = getInvertedValue(source, source->Maximum);
            target->Maximum = getInvertedValue(source, source->Minimum);
        }

        void InvertedDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            const double invertedMean = this->getInvertedValue(stochast, mean);
            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);

            this->innerDistribution->setMeanAndDeviation(invertedStochast, invertedMean, deviation);

            copyFromInverted(stochast, invertedStochast);
        }

        void InvertedDistribution::setShift(std::shared_ptr<StochastProperties> stochast, double shift, bool inverted)
        {
            this->innerDistribution->setShift(stochast, shift, true);
        }

        bool InvertedDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);

            return this->innerDistribution->isVarying(invertedStochast);
        }

        double InvertedDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            return this->getInvertedValue(stochast, this->innerDistribution->getMean(invertedStochast));
        }

        double InvertedDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            return this->innerDistribution->getDeviation(invertedStochast);
        }

        double InvertedDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            const double xInvert = this->innerDistribution->getXFromU(invertedStochast, -u);

            return this->getInvertedValue(stochast, xInvert);
        }

        double InvertedDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            const double xInvert = this->getInvertedValue(stochast, x);

            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            return - this->innerDistribution->getUFromX(invertedStochast, xInvert);
        }

        double InvertedDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            const double xInvert = this->getInvertedValue(stochast, x);

            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            return this->innerDistribution->getPDF(invertedStochast, xInvert);
        }

        double InvertedDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            const double xInvert = this->getInvertedValue(stochast, x);

            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            return 1 - this->innerDistribution->getCDF(invertedStochast, xInvert);
        }

        void InvertedDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            double xInvert = this->getInvertedValue(stochast, x);

            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            this->innerDistribution->setXAtU(invertedStochast, xInvert, -u, constantType);

            copyFromInverted(stochast, invertedStochast);
        }

        void InvertedDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            // fit the shift first
            // do not use inverted value, because it depends on stochast->Shift, which is not known yet (because it has to be fitted)
            if (this->innerDistribution->isShiftUsed())
            {
                std::vector<double> zeroInvertedValues = Numeric::NumericSupport::select(values, [](double x) {return -x; });

                const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
                this->innerDistribution->fit(invertedStochast, zeroInvertedValues);

                stochast->Shift = -stochast->Shift;
            }

            std::vector<double> invertedValues = Numeric::NumericSupport::select(values, [this, stochast](double x) {return this->getInvertedValue(stochast, x); });

            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            this->innerDistribution->fit(invertedStochast, invertedValues);

            copyFromInverted(stochast, invertedStochast);
        }

        void InvertedDistribution::fitPrior(const std::shared_ptr<StochastProperties>& stochast, const std::shared_ptr<StochastProperties>& prior, std::vector<double>& values)
        {
            // fit the shift first
            // do not use inverted value, because it depends on stochast->Shift, which is not known yet (because it has to be fitted)
            if (this->innerDistribution->isShiftUsed())
            {
                std::vector<double> zeroInvertedValues = Numeric::NumericSupport::select(values, [](double x) {return -x; });

                const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
                const std::shared_ptr<StochastProperties> invertedPrior = getInvertedStochast(prior);

                this->innerDistribution->fitPrior(invertedStochast, invertedPrior, zeroInvertedValues);

                stochast->Shift = -stochast->Shift;
            }

            std::vector<double> invertedValues = Numeric::NumericSupport::select(values, [this, stochast](double x)
            {
                return this->getInvertedValue(stochast, x);
            });

            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            const std::shared_ptr<StochastProperties> invertedPrior = getInvertedStochast(prior);

            this->innerDistribution->fitPrior(invertedStochast, invertedPrior, invertedValues);

            copyFromInverted(stochast, invertedStochast);
        }

        void InvertedDistribution::validate(Logging::ValidationReport& report, std::shared_ptr<StochastProperties> stochast)
        {
            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            this->innerDistribution->validate(report, invertedStochast);
        }

        double InvertedDistribution::getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x)
        {
            const double xInvert = this->getInvertedValue(stochast, x);

            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            return this->innerDistribution->getLogLikelihood(invertedStochast, xInvert);
        }

        std::vector<double> InvertedDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            const std::shared_ptr<StochastProperties> invertedStochast = getInvertedStochast(stochast);
            std::vector<double> specialPoints = this->innerDistribution->getSpecialPoints(invertedStochast);

            return Numeric::NumericSupport::select(specialPoints, [this, stochast](double x) {return this->getInvertedValue(stochast, x); });
        }

        std::vector<DistributionPropertyType> InvertedDistribution::getParameters()
        {
            return innerDistribution->getParameters();
        }
    }
}

