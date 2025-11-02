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
#include "ConditionalWeibullDistribution.h"

#include <cmath>

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include "DistributionFitter.h"
#include "DistributionSupport.h"


namespace Deltares
{
    namespace Statistics
    {
        void ConditionalWeibullDistribution::initialize(StochastProperties& stochast, const std::vector<double>& values)
        {
            // not supported
        }

        void ConditionalWeibullDistribution::validate(Logging::ValidationReport& report, std::shared_ptr<StochastProperties> stochast, std::string& subject)
        {
            Logging::ValidationSupport::checkMinimumNonInclusive(report, 0, stochast->Scale, "scale", subject);
            Logging::ValidationSupport::checkMinimumNonInclusive(report, 0, stochast->Shape, "shape", subject);
            Logging::ValidationSupport::checkMinimumNonInclusive(report, 0, stochast->ShapeB, "shape B", subject);
        }

        bool ConditionalWeibullDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return isValid(*stochast);
        }

        double ConditionalWeibullDistribution::getMean(StochastProperties& stochast)
        {
            return DistributionSupport::getMeanByIteration(*this, stochast);
        }

        double ConditionalWeibullDistribution::getDeviation(StochastProperties& stochast)
        {
            return DistributionSupport::getDeviationByIteration(*this, stochast);
        }

        double ConditionalWeibullDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            const double qMin = 1.0e-300;

            PQ pq = StandardNormal::getPQFromU(u);

            if (pq.q == 1.0)
            {
                return stochast.Shift;
            }

            double f; // Exceeding frequency

            if (pq.q <= tresholdF)
            {
                f = std::max(pq.q, qMin);
            }
            else
            {
                double pd = std::max(pq.p, qMin);
                f = -log(pd);
            }

            const double logF = std::log(f / stochast.ShapeB);
            const double xlog = std::pow(stochast.Shift / stochast.Scale, stochast.Shape) - logF;
            if (xlog <= 0.0) return 0.0;
            const double xScale = pow(xlog, 1.0 / stochast.Shape);

            return xScale * stochast.Scale;
        }

        double ConditionalWeibullDistribution::getExponent(const StochastProperties& stochast, double x)
        {
            x /= stochast.Scale;
            if (x <= 0.0) return -StandardNormal::BetaMax;
            const double xLog = pow(x, stochast.Shape);
            const double logF = std::pow(stochast.Shift / stochast.Scale, stochast.Shape) - xLog;
            const double f = stochast.ShapeB * exp(logF);

            return f;
        }

        double ConditionalWeibullDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            double f = getExponent(stochast, x);

            double u;
            if (f < tresholdF)
            {
                u = StandardNormal::getUFromQ(f);
            }
            else
            {
                double pd = exp(-f);
                u = StandardNormal::getUFromP(pd);
            }
            return u;
        }

        double ConditionalWeibullDistribution::getPDF(StochastProperties& stochast, double x)
        {
            const double pdf = this->getCDF(stochast, x)
                * getExponent(stochast, x) * stochast.Shape * pow(x / stochast.Scale, stochast.Shape - 1.0) / stochast.Scale;

            return pdf;
        }

        double ConditionalWeibullDistribution::getCDF(StochastProperties& stochast, double x)
        {
            const double u = getUFromX(stochast, x);
            return StandardNormal::getPFromU(u);
        }

        void ConditionalWeibullDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            if (stochast.Shape != 1.0 || stochast.ShapeB != 1.0)
            {
                constexpr double toleranceBisection = 0.00001;
                auto bisection = Numeric::BisectionRootFinder(toleranceBisection);

                Numeric::RootFinderMethod method = [&stochast, this](double s)
                {
                    stochast.Scale = s;
                    return getMean(stochast);
                };

                double minStart = 0.5 * stochast.Scale;
                double maxStart = 1.5 * stochast.Scale;

                stochast.Scale = bisection.CalculateValue(minStart, maxStart, mean, method);
            }
        }

        void ConditionalWeibullDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            DistributionSupport::setXAtUByIteration(*this, stochast, x, u, constantType);
        }

        void ConditionalWeibullDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift)
        {
            double minValue = Numeric::NumericSupport::getMinimum(values);
            double meanValue = Numeric::NumericSupport::getMean(values);
            double estimatedMaxValue = minValue + 2 * (meanValue - minValue);

            std::shared_ptr<DistributionFitter> fitter = std::make_shared<DistributionFitter>();

            std::vector<double> minValues = { minValue, 0.5, 0.5, 0.5 };
            std::vector<double> maxValues = { estimatedMaxValue, 1.5, 1.5, 1.5 };
            std::vector<double> initValues = { meanValue, 1.0, 1.0, 1.0 };
            std::vector<DistributionPropertyType> properties = { Shift, Scale, Shape, ShapeB };
            std::vector<double> parameters = fitter->fitByLogLikelihood(values, this, stochast, minValues, maxValues, initValues, properties);

            stochast->Shift = std::max(minValue, parameters[0]);
            stochast->Scale = std::max(0.0, parameters[1]);
            stochast->Shape = std::max(0.0, parameters[2]);
            stochast->ShapeB = std::max(0.0, parameters[3]);

            stochast->Observations = static_cast<int>(values.size());
        }

        std::vector<double> ConditionalWeibullDistribution::getSpecialPoints(StochastProperties& stochast)
        {
            std::vector<double> specialPoints{ stochast.Shift };
            return specialPoints;
        }
    }
}



