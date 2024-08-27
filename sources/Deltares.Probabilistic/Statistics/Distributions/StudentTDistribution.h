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
            void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values) override;
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

