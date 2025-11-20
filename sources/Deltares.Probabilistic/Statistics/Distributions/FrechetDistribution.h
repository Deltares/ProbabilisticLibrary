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
#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        class FrechetDistribution : public Distribution
        {
            void initialize(StochastProperties& stochast, const std::vector<double>& values) override;
            double getXFromU(StochastProperties& stochast, double u) override;
            double getUFromX(StochastProperties& stochast, double x) override;
            bool isVarying(StochastProperties& stochast) override;
            bool canTruncate() override { return true; }
            bool canInvert() override { return true; }
            double getMean(StochastProperties& stochast) override;
            double getDeviation(StochastProperties& stochast) override;
            void setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation) override;
            double getPDF(StochastProperties& stochast, double x) override;
            double getCDF(StochastProperties& stochast, double x) override;
            void setXAtU(StochastProperties& stochast, double x, double u, ConstantParameterType constantType) override;
            bool canFit() override { return true; }
            void fit(StochastProperties& stochast, const std::vector<double>& values, const double shift) override;
            bool useShiftForInversion() override { return false; }
            double getMaxShiftValue(std::vector<double>& values) override;
            std::vector<double> getSpecialPoints(StochastProperties& stochast) override;
            std::vector<DistributionPropertyType> getParameters() override
            {
                using enum DistributionPropertyType;
                return {Shift, Scale, Shape };
            }
        };
    }
}
