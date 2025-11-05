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
#include <cmath>
#include "StudentTDistribution.h"
#include "../StochastProperties.h"
#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/SpecialFunctions.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include "../../Utils/Locker.h"

#include <numbers>

#include "DistributionSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        using Numeric::NumericSupport;

        void StudentTDistribution::initialize(StochastProperties& stochast, const std::vector<double>& values)
        {
            setMeanAndDeviation(stochast, values[0], values[1]);
        }

        bool StudentTDistribution::isVarying(StochastProperties& stochast)
        {
            return stochast.Scale > 0.0;
        }

        double StudentTDistribution::getMean(StochastProperties& stochast)
        {
            return stochast.Location;
        }

        double StudentTDistribution::getDeviation(StochastProperties& stochast)
        {
            if (stochast.Observations > 3)
            {
                double observationFactor = sqrt(Numeric::NumericSupport::Divide(stochast.Observations - 1, stochast.Observations - 3));
                return stochast.Scale * observationFactor;
            }
            else
            {
                return std::numeric_limits<double>::infinity();
            }
        }

        void StudentTDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            stochast.Location = mean;

            if (stochast.Observations > 3)
            {
                double observationFactor = sqrt(Numeric::NumericSupport::Divide(stochast.Observations - 1, stochast.Observations - 3));
                stochast.Scale = deviation / observationFactor;
            }
        }

        double StudentTDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            int degreesOfFreedom = stochast.Observations - 1;

            bool success;
            auto studentValue = GetStudentValue(degreesOfFreedom, success);

            if ( ! success)
            {
                return std::nan("");
            }

            double p = StandardNormal::getPFromU(u);
            double c = studentValue.getCoefficient(p);

            return stochast.Location - c * stochast.Scale;
        }

        double StudentTDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            double margin = std::min(minXDelta, std::fabs(x / 1000000.0));

            int degreesOfFreedom = stochast.Observations - 1;

            bool success;
            auto studentValue = GetStudentValue(degreesOfFreedom, success);

            if ( ! success)
            {
                return std::nan("");
            }

            auto bisection = Numeric::BisectionRootFinder(margin);

            Numeric::RootFinderMethod function = [stochast, &studentValue](double u)
            {
                double p = StandardNormal::getPFromU(u);
                double c = studentValue.getCoefficient(p);

                return stochast.Location - c * stochast.Scale;
            };

            double result = bisection.CalculateValue(-1, 1, x, function);

            return result;
        }

        void StudentTDistribution::setXAtU(StochastProperties& stochast, double x, double u, ConstantParameterType constantType)
        {
            if (constantType == ConstantParameterType::Deviation)
            {
                const double current = getXFromU(stochast, u);
                const double diff = x - current;

                stochast.Location += diff;
            }
            else if (constantType == ConstantParameterType::VariationCoefficient)
            {
                DistributionSupport::setXAtUByIteration(*this, stochast, x, u, constantType);
            }
        }

        double StudentTDistribution::getPDF(StochastProperties& stochast, double x)
        {
            const int degreesOfFreedom = stochast.Observations - 1;

            const double xmu = (x - stochast.Location) / stochast.Scale;

            const double gamma = Numeric::SpecialFunctions::getGamma(0.5 * stochast.Observations);
            const double factor = std::sqrt(std::numbers::pi * degreesOfFreedom) * Numeric::SpecialFunctions::getGamma(0.5 * degreesOfFreedom) * stochast.Scale;
            const double exp = std::pow(1.0 + xmu * xmu / degreesOfFreedom, -0.5 * stochast.Observations);

            return gamma * exp / factor;
        }

        double StudentTDistribution::getCDF(StochastProperties& stochast, double x)
        {
            const double u = this->getUFromX(stochast, x);

            return StandardNormal::getPFromU(u);
        }

        void StudentTDistribution::fit(StochastProperties& stochast, const std::vector<double>& values, const double shift)
        {
            constexpr double minSigma = 1.0E-9;
            constexpr double minRelativeSigma = 0.99;
            constexpr double maxRelativeSigma = 1.01;
            constexpr int maxIterations = 25;

            std::vector<double> weights = NumericSupport::select(values, [](double x) {return 1.0; });

            stochast.Observations = static_cast<int>(values.size());

            int nu = static_cast<int>(values.size()) - 1;

            double prevSigma2 = 0;
            int count = 0;

            while (true)
            {
                // EM algorithm
                // https://stats.stackexchange.com/questions/63647/estimating-parameters-of-students-t-distribution

                double mean = NumericSupport::getWeightedMean(values, weights);

                std::vector<double> sigma2Values = NumericSupport::zip(values, weights, [mean](double xx, double ww)
                {
                    return ww * (xx - mean) * (xx - mean);
                });

                double sigma2 = NumericSupport::sum(sigma2Values) / stochast.Observations;

                weights = NumericSupport::select(values, [nu, sigma2, mean](double xx)
                {
                    return (nu + 1) * sigma2 / (nu * sigma2 + (xx - mean) * (xx - mean));
                });

                if (sigma2 < minSigma || (prevSigma2 / sigma2 > minRelativeSigma && prevSigma2 / sigma2 < maxRelativeSigma) || count > maxIterations)
                {
                    stochast.Location = mean;
                    stochast.Scale = std::sqrt(sigma2);
                    break;
                }
                else
                {
                    prevSigma2 = sigma2;
                    count++;
                }
            }
        }

        StudentTDistribution::StudentTValue StudentTDistribution::GetStudentValue(int degreesOfFreedom, bool& success) const
        {
            if (degreesOfFreedom <= 0)
            {
                success = false;
                return {};
            }

            for (size_t i = 0; i < studentValues.size(); i++)
            {
                auto studentValue = studentValues[i];

                if (studentValue.N >= degreesOfFreedom)
                {
                    success = true;
                    if (studentValue.N == degreesOfFreedom || studentValue.N == std::numeric_limits<int>::max())
                    {
                        return studentValue;
                    }
                    else
                    {
                        // linear interpolation
                        double fraction = NumericSupport::Divide(degreesOfFreedom - studentValues[i - 1].N,
                                                            studentValues[i].N - studentValues[i - 1].N);
                        auto newStudentValue = StudentTValue(
                            degreesOfFreedom,
                            (1.0 - fraction) * studentValues[i - 1].P0_100 + fraction * studentValues[i].P0_100,
                            (1.0 - fraction) * studentValues[i - 1].P0_050 + fraction * studentValues[i].P0_050,
                            (1.0 - fraction) * studentValues[i - 1].P0_025 + fraction * studentValues[i].P0_025,
                            (1.0 - fraction) * studentValues[i - 1].P0_010 + fraction * studentValues[i].P0_010,
                            (1.0 - fraction) * studentValues[i - 1].P0_005 + fraction * studentValues[i].P0_005);

                        return newStudentValue;
                    }
                }
            }

            success = false;
            return {};
        }

        double StudentTDistribution::StudentTValue::getCoefficient(double p)
        {
            if (Numeric::NumericSupport::areEqual(p, 0.5, pDelta))
            {
                return 0;
            }
            else if (p > 0.5)
            {
                return -getCoefficient(1 - p);
            }
            else if (Numeric::NumericSupport::areEqual(p, 0.1, pDelta))
            {
                return P0_100;
            }
            else if (Numeric::NumericSupport::areEqual(p, 0.05, pDelta))
            {
                return P0_050;
            }
            else if (Numeric::NumericSupport::areEqual(p, 0.025, pDelta))
            {
                return P0_025;
            }
            else if (Numeric::NumericSupport::areEqual(p, 0.010, pDelta))
            {
                return P0_010;
            }
            else if (Numeric::NumericSupport::areEqual(p, 0.005, pDelta))
            {
                return P0_005;
            }
            else
            {
                double c = Numeric::NumericSupport::interpolate(StandardNormal::getUFromP(p), getPValues(p), cValues);
                return c;
            }
        }

        std::vector<double> StudentTDistribution::StudentTValue::getPValues(double maxP)
        {
            if (pValues.empty() || pValues[0] > StandardNormal::getUFromP(maxP))
            {
                std::vector<double> p = { StandardNormal::getUFromP(0.5), StandardNormal::getUFromP(0.1), StandardNormal::getUFromP(0.05), StandardNormal::getUFromP(0.025), StandardNormal::getUFromP(0.01), StandardNormal::getUFromP(0.005) };
                std::vector<double> n = { 0.5, 0.1, 0.05, 0.025, 0.01, 0.005 };

                while (n[n.size() - 1] > maxP)
                {
                    double newN = n[n.size() - 1] / 2.0;
                    n.push_back(newN);
                    p.push_back(StandardNormal::getUFromP(newN));
                }

                std::vector<double> c = { 0.0, P0_100, P0_050, P0_025, P0_010, P0_005 };
                while (c.size() < n.size())
                {
                    c.push_back(Numeric::NumericSupport::interpolate(p[c.size()], { p[c.size() - 1], p[c.size() - 2] }, { c[c.size() - 1], c[c.size() - 2] }, true));
                }

                std::reverse(p.begin(), p.end());
                std::reverse(c.begin(), c.end());

                pValues = p;
                cValues = c;
            }

            return pValues;
        }

        void StudentTDistribution::loadValues()
        {
            studentValues.emplace_back(1, 3.078, 6.314, 12.706, 31.821, 63.657);
            studentValues.emplace_back(2, 1.886, 2.92, 4.303, 6.965, 9.925);
            studentValues.emplace_back(3, 1.638, 2.353, 3.182, 4.541, 5.841);
            studentValues.emplace_back(4, 1.533, 2.132, 2.776, 3.747, 4.604);
            studentValues.emplace_back(5, 1.476, 2.015, 2.571, 3.365, 4.032);
            studentValues.emplace_back(6, 1.44, 1.943, 2.447, 3.143, 3.707);
            studentValues.emplace_back(7, 1.415, 1.895, 2.365, 2.998, 3.499);
            studentValues.emplace_back(8, 1.397, 1.86, 2.306, 2.896, 3.355);
            studentValues.emplace_back(9, 1.383, 1.833, 2.262, 2.821, 3.25);
            studentValues.emplace_back(10, 1.372, 1.812, 2.228, 2.764, 3.169);
            studentValues.emplace_back(11, 1.363, 1.796, 2.201, 2.718, 3.106);
            studentValues.emplace_back(12, 1.356, 1.782, 2.179, 2.681, 3.055);
            studentValues.emplace_back(13, 1.35, 1.771, 2.16, 2.65, 3.012);
            studentValues.emplace_back(14, 1.345, 1.761, 2.145, 2.624, 2.977);
            studentValues.emplace_back(15, 1.341, 1.753, 2.131, 2.602, 2.947);
            studentValues.emplace_back(16, 1.337, 1.746, 2.12, 2.583, 2.921);
            studentValues.emplace_back(17, 1.333, 1.74, 2.11, 2.567, 2.898);
            studentValues.emplace_back(18, 1.33, 1.734, 2.101, 2.552, 2.878);
            studentValues.emplace_back(19, 1.328, 1.729, 2.093, 2.539, 2.861);
            studentValues.emplace_back(20, 1.325, 1.725, 2.086, 2.528, 2.845);
            studentValues.emplace_back(21, 1.323, 1.721, 2.08, 2.518, 2.831);
            studentValues.emplace_back(22, 1.321, 1.717, 2.074, 2.508, 2.819);
            studentValues.emplace_back(23, 1.319, 1.714, 2.069, 2.5, 2.807);
            studentValues.emplace_back(24, 1.318, 1.711, 2.064, 2.492, 2.797);
            studentValues.emplace_back(25, 1.316, 1.708, 2.06, 2.485, 2.787);
            studentValues.emplace_back(26, 1.315, 1.706, 2.056, 2.479, 2.779);
            studentValues.emplace_back(27, 1.314, 1.703, 2.052, 2.473, 2.771);
            studentValues.emplace_back(28, 1.313, 1.701, 2.048, 2.467, 2.763);
            studentValues.emplace_back(29, 1.311, 1.699, 2.045, 2.462, 2.756);
            studentValues.emplace_back(30, 1.31, 1.697, 2.042, 2.457, 2.75);
            studentValues.emplace_back(40, 1.303, 1.684, 2.021, 2.423, 2.704);
            studentValues.emplace_back(60, 1.296, 1.671, 2, 2.39, 2.66);
            studentValues.emplace_back(120, 1.289, 1.658, 1.98, 2.358, 2.617);
            studentValues.emplace_back(std::numeric_limits<int>::max(), 1.282, 1.645, 1.96, 2.326, 2.576);
        }
    }
}




