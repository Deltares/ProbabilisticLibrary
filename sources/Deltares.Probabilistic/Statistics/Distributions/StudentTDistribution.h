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

namespace Deltares::Utils
{
    class Locker;
}

namespace Deltares
{
    namespace Statistics
    {
        class StudentTDistribution : public Distribution
        {
        public:
            void initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values) override;
            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
            bool canTruncate() override { return true; }
            bool canFit() override { return true; }
            double getMean(std::shared_ptr<StochastProperties> stochast) override;
            double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
            void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) override;
            double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            void setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType) override;
            void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift = nan("")) override;
            std::vector<DistributionPropertyType> getParameters() override { return { Location, Scale, Observations }; }
        private:
            class StudentTValue
            {
            public:
                StudentTValue(int N, double P0_100, double P0_050, double P0_025, double P0_010, double P0_005)
                {
                    this->N = N;
                    this->P0_100 = P0_100;
                    this->P0_050 = P0_050;
                    this->P0_025 = P0_025;
                    this->P0_010 = P0_010;
                    this->P0_005 = P0_005;
                }

                int N = 0;
                double P0_100 = 0.0;
                double P0_050 = 0.0;
                double P0_025 = 0.0;
                double P0_010 = 0.0;
                double P0_005 = 0.0;

                double getCoefficient(double p);

            private:
                static constexpr double pDelta = 0.0001;

                std::vector<double> pValues;
                std::vector<double> cValues;
                std::vector<double> getPValues(double maxP);
            };

            std::shared_ptr<StudentTDistribution::StudentTValue> GetStudentValue(int degreesOfFreedom);
            std::vector<std::shared_ptr<StudentTDistribution::StudentTValue>> values;
            void loadValues();

            const double minXDelta = 0.00001;
        };
    }
}

