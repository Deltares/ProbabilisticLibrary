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
#include "FORMS.h"

#include <algorithm>
#include <vector>
#include <cmath>
#include <memory>

#include "../Math/NumericSupport.h"
#include "../Model/Sample.h"
#include "../Model/GradientCalculator.h"
#include "../Statistics/DistributionType.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/FragilityValue.h"

using namespace Deltares::Models;

namespace Deltares
{
    namespace Sensitivity
    {
        using namespace Deltares::Models;
        using namespace Deltares::Statistics;

        std::shared_ptr<Sensitivity::SensitivityResult> FORMS::getSensitivityStochast(std::shared_ptr<ModelRunner> modelRunner)
        {
            int nStochasts = modelRunner->getVaryingStochastCount();

            std::shared_ptr<Models::GradientCalculator> gradientCalculator = std::make_shared<GradientCalculator>();
            gradientCalculator->Settings = std::make_shared<GradientSettings>();
            gradientCalculator->Settings = Settings->GradientSettings;

            int iteration = 0;
            double zPrevious = std::nan("");
            bool ascending = true;
            double factorBeta = 1;

            double requiredBetaIncrement = Settings->StepSize / 5;

            int maxIterations = static_cast<int>((this->Settings->Maximum - this->Settings->Minimum) / this->Settings->StepSize) + 1;
            int performedIterations = 0;

            // holds results
            std::shared_ptr<Stochast> cdfCurve = std::make_shared<Stochast>();
            cdfCurve->setDistributionType(DistributionType::CDFCurve);

            std::shared_ptr<Sample> startPoint = std::make_shared<Sample>(nStochasts);
            startPoint->IterationIndex = iteration - 1;

            std::vector<double> gradient0 = gradientCalculator->getGradient(modelRunner, startPoint);
            double z0 = startPoint->Z;

            std::shared_ptr<FragilityValue> zeroValue = std::make_shared<FragilityValue>();
            zeroValue->X = z0;
            zeroValue->Reliability = 0;
            cdfCurve->getProperties()->FragilityValues.push_back(zeroValue);

            bool valid = isZValid(modelRunner, z0) && isGradientValid(modelRunner, gradient0);

            modelRunner->reportProgress(++performedIterations, maxIterations);

            // ascending part
            if (valid && Settings->Maximum > 0)
            {
                while (startPoint->getBeta() <= Settings->Maximum)
                {
                    std::vector<double> gradient = gradient0;

                    double z = z0;

                    if (iteration > 0)
                    {
                        startPoint->IterationIndex = iteration - 1;
                        gradient = gradientCalculator->getGradient(modelRunner, startPoint);
                        z = startPoint->Z;

                        modelRunner->reportProgress(++performedIterations, maxIterations);

                        if (!isZValid(modelRunner, z))
                        {
                            break;
                        }
                        if (!isGradientValid(modelRunner, gradient))
                        {
                            break;
                        }

                        if (iteration == 1)
                        {
                            ascending = z - zPrevious >= 0;
                            factorBeta = ascending ? 1 : -1;
                        }
                        else
                        {
                            if (!isMonotone(modelRunner, z, zPrevious, ascending))
                            {
                                break;
                            }
                        }
                    }

                    zPrevious = z;
                    double betaPrevious = startPoint->getBeta();

                    if (iteration > 0)
                    {
                        std::shared_ptr<FragilityValue> fragilityValue = std::make_shared<FragilityValue>();
                        fragilityValue->X = z;
                        fragilityValue->Reliability = factorBeta * startPoint->getBeta();
                        cdfCurve->getProperties()->FragilityValues.push_back(fragilityValue);
                    }

                    double stepSize = Settings->StepSize;
                    if (iteration == 0)
                    {
                        stepSize = std::max(stepSize, Settings->Minimum);
                    }

                    double factor = stepSize / Numeric::NumericSupport::GetLength(gradient);

                    std::vector<double> nextU = Numeric::NumericSupport::zip(startPoint->Values, gradient,[factor](double p, double q) { return p + q * factor; });

                    startPoint = std::make_shared<Sample>(nextU);

                    if (!isBetaValid(modelRunner, startPoint->getBeta(), betaPrevious, requiredBetaIncrement))
                    {
                        break;
                    }

                    iteration++;
                }

                const double tolerance = 1E-6;
                valid &= Numeric::NumericSupport::isGreater(startPoint->getBeta(), Settings->Maximum, tolerance);
            }


            if (valid && Settings->Minimum < 0)
            {
                // reset start point
                iteration = 0;
                ascending = !ascending;
                factorBeta = -factorBeta;
                zPrevious = z0;

                startPoint = std::make_shared<Sample>(nStochasts);

                while (startPoint->getBeta() <= std::abs(Settings->Minimum))
                {
                    std::vector<double> gradient = gradient0;
                    double z = z0;

                    if (iteration < 0)
                    {
                        startPoint->IterationIndex = iteration - 1;
                        gradient = gradientCalculator->getGradient(modelRunner, startPoint);
                        z = startPoint->Z;

                        modelRunner->reportProgress(++performedIterations, maxIterations);

                        if (!isZValid(modelRunner, z))
                        {
                            break;
                        }
                        if (!isGradientValid(modelRunner, gradient))
                        {
                            break;
                        }
                        if (!isMonotone(modelRunner, z, zPrevious, ascending))
                        {
                            break;
                        }
                    }

                    zPrevious = z;
                    double betaPrevious = startPoint->getBeta();

                    if (iteration < 0)
                    {
                        std::shared_ptr<FragilityValue> fragilityValue = std::make_shared<FragilityValue>();
                        fragilityValue->X = z;
                        fragilityValue->Reliability = factorBeta * startPoint->getBeta();
                        cdfCurve->getProperties()->FragilityValues.push_back(fragilityValue);
                    }

                    double stepSize = Settings->StepSize;
                    if (iteration == 0)
                    {
                        stepSize = std::max(stepSize, -Settings->Maximum);
                    }

                    double factor = stepSize / Numeric::NumericSupport::GetLength(gradient);

                    std::vector<double> nextU = Numeric::NumericSupport::zip(startPoint->Values, gradient, [factor](double p, double q) { return p - q * factor; });
                    startPoint = std::make_shared<Sample>(nextU);

                    if (!isBetaValid(modelRunner, startPoint->getBeta(), betaPrevious, requiredBetaIncrement))
                    {
                        break;
                    }

                    iteration--;
                }
            }

            if (cdfCurve->getProperties()->FragilityValues.size() <= 1)
            {
                std::shared_ptr<Statistics::Stochast> stochast = std::make_shared<Stochast>(DistributionType::Deterministic, std::vector<double> { z0 });
                return modelRunner->getSensitivityResult(stochast);
            }
            else
            {
                std::sort(cdfCurve->getProperties()->FragilityValues.begin(), cdfCurve->getProperties()->FragilityValues.end(),
                    [](std::shared_ptr<FragilityValue> p, std::shared_ptr<FragilityValue> q) {return p->X < q->X; });

                return modelRunner->getSensitivityResult(cdfCurve);
            }
        }

        bool FORMS::isZValid(std::shared_ptr<ModelRunner> modelRunner, double z)
        {
            if (std::isnan(z))
            {
                modelRunner->reportMessage(MessageType::Error, "Model result is not valid");
                return false;
            }

            return true;
        }

        bool FORMS::isMonotone(std::shared_ptr<ModelRunner> modelRunner, double z, double zPrevious, bool ascending)
        {
            if (z - zPrevious >= 0 != ascending)
            {
                modelRunner->reportMessage(MessageType::Error, "Model is not monotone");
                return false;
            }

            return true;
        }

        bool FORMS::isGradientValid(std::shared_ptr<ModelRunner> modelRunner, std::vector<double>& gradient)
        {
            if (!areResultsValid(gradient))
            {
                modelRunner->reportMessage(MessageType::Error, "Model did not return valid results");
                return false;
            }

            repairResults(gradient);

            double gradientLength = Numeric::NumericSupport::GetLength(gradient);
            if (gradientLength < 1E-6)
            {
                modelRunner->reportMessage(MessageType::Error, "Model has no gradient");
                return false;
            }

            return true;
        }

        bool FORMS::isBetaValid(std::shared_ptr<ModelRunner> modelRunner, double beta, double betaPrevious, double requiredBetaIncrement)
        {
            if (beta < betaPrevious)
            {
                modelRunner->reportMessage(MessageType::Error, "Beta decreases with next step");
                return false;
            }
            else if (beta < betaPrevious + requiredBetaIncrement)
            {
                modelRunner->reportMessage(MessageType::Error, "Beta does not increase enough with step");
                return false;
            }

            return true;
        }

        void FORMS::repairResults(std::vector<double>& values)
        {
            for (auto& value : values)
            {
                if (std::isnan(value))
                {
                    value = 0;
                }
            }
        }

        bool FORMS::areResultsValid(std::vector<double>& values)
        {
            for (auto& value : values)
            {
                if (!std::isnan(value))
                {
                    return true;
                }
            }

            return false;
        }
    }
}



