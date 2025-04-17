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

#include "DirectionSectionsCalculationDS.h"
#include "../Math/NumericSupport.h"
#include "../Math/RootFinders/LinearRootFinder.h"
#include "../Math/RootFinders/BisectionRootFinder.h"

namespace Deltares::Reliability
{
    using namespace Deltares::Numeric;

    double DirectionSectionsCalculationDS::findBetaBetweenBoundaries(Models::ModelRunner& modelRunner,
        const DirectionCalculation& directionCalculation,
        double uLow, double uHigh, double zLow, double zHigh, double& z)
    {
        if (std::isnan(zLow) || std::isnan(zHigh))
        {
            return findBetaBetweenBoundariesAllowNaN(directionCalculation, uLow, uHigh, zLow, zHigh, z);
        }
        else
        {
            const double zTolerance = GetZTolerance(Settings, uLow, uHigh, zLow, zHigh);

            auto linearSearchCalculation = LinearRootFinder(zTolerance, Settings.MaximumIterations);

            auto low = XValue(uLow, zLow);
            auto high = XValue(uHigh, zHigh);
            auto resultRootFinder = linearSearchCalculation.CalculateValue(low, high, 0.0, [directionCalculation](double v) { return directionCalculation.GetZ(v); });

            z = std::isnan(resultRootFinder.X) ? nan("") : resultRootFinder.Value;

            if (modelRunner.Settings->IsProxyModel())
            {
                if (std::isnan(resultRootFinder.X))
                {
                    auto bisectionCalculation = BisectionRootFinder(zTolerance);
                    resultRootFinder.X = bisectionCalculation.CalculateValue(uLow, uHigh, 0.0, [directionCalculation](double v) { return directionCalculation.GetZ(v); });
                }

                const auto ThresholdOffset = modelRunner.Settings->proxySettings->ThresholdOffset;
                if (modelRunner.Settings->proxySettings->ShouldUpdateFinalSteps && !isProxyAllowed(ThresholdOffset, resultRootFinder.X, this->Threshold))
                {
                    directionCalculation.uDirection.AllowProxy = false;

                    double z0 = directionCalculation.GetZProxy(0, false);
                    double zResult = directionCalculation.GetZProxy(resultRootFinder.X, false);

                    if (std::isnan(zResult))
                    {
                        z = zResult;
                        modelRunner.removeTask(directionCalculation.uDirection.IterationIndex);
                        return Settings.MaximumLengthU;
                    }
                    else if (NumericSupport::GetSign(z0) == NumericSupport::GetSign(zResult) && std::fabs(zResult) >= std::fabs(z0))
                    {
                        z = zResult;
                        modelRunner.removeTask(directionCalculation.uDirection.IterationIndex);
                        return Settings.MaximumLengthU;
                    }
                    else
                    {
                        double uNew = NumericSupport::interpolate(0, z0, 0, zResult, resultRootFinder.X, true);
                        if (isProxyAllowed(ThresholdOffset, uNew, Threshold))
                        {
                            z = zResult;
                            return std::min(uNew, Settings.MaximumLengthU);
                        }
                    }

                    auto lowProxy = XValue(0.0, z0);
                    auto highProxy = XValue(resultRootFinder.X, zResult);
                    resultRootFinder = linearSearchCalculation.CalculateValue(lowProxy, highProxy, 0.0, [directionCalculation](double v) { return directionCalculation.GetZNoProxy(v); });
                    if (std::isnan(resultRootFinder.X))
                    {
                        z = zResult;
                        resultRootFinder.X = Settings.MaximumLengthU;
                    }
                    else
                    {
                        z = directionCalculation.GetZProxy(resultRootFinder.X, false);
                    }
                }
            }

            return resultRootFinder.X;
        }
    }

    bool DirectionSectionsCalculationDS::isProxyAllowed(double ThresholdOffset, double u, double threshold)
    {
        return std::isnan(u) || u > threshold + ThresholdOffset;
    }
}

