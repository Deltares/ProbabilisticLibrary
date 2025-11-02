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
        class BernoulliDistribution : public Distribution
        {
        public:
            void initialize(StochastProperties& stochast, const std::vector<double>& values) override;
            double getXFromU(StochastProperties& stochast, double u) override;
            double getUFromX(StochastProperties& stochast, double x) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
            void validate(Logging::ValidationReport& report, std::shared_ptr<StochastProperties> stochast, std::string& subject) override;
            double getMean(StochastProperties& stochast) override;
            double getDeviation(StochastProperties& stochast) override;
            void setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation) override;
            double getPDF(StochastProperties& stochast, double x) override;
            double getCDF(StochastProperties& stochast, double x) override;
            bool canFit() override { return true; }
            bool canFitPrior() override { return true; }
            void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift) override;
            void fitPrior(const std::shared_ptr<StochastProperties>& stochast, std::vector<double>& values, const std::shared_ptr<StochastProperties>& prior, const double shift) override;
            std::vector<double> getDiscontinuityPoints(const StochastProperties& stochast) override;
            std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
            std::vector<DistributionPropertyType> getParameters() override { return { Location, Observations }; }
        private:
            const double delta = 0.0000001;
        };
    }
}

