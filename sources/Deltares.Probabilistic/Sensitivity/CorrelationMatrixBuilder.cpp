#include "CorrelationMatrixBuilder.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Sensitivity
    {
        void CorrelationMatrixBuilder::registerWeights(const std::vector<double>& weights)
        {
            this->weights = weights;
        }

        void CorrelationMatrixBuilder::registerSamples(const std::shared_ptr<Statistics::Stochast> stochast, const std::vector<double>& values)
        {
            this->stochasts.push_back(stochast);

            stochast->initializeForRun();

            std::vector<double> uValues;

            for (size_t i = 0; i < values.size(); i++)
            {
                uValues.push_back(stochast->getUFromX(values[i]));
            }

            this->stochastValues[stochast] = uValues;
        };

        double CorrelationMatrixBuilder::getCorrelationValue(std::shared_ptr<Statistics::Stochast> x, std::shared_ptr<Statistics::Stochast> y)
        {
            double sumCross = 0;
            double sumProductsX = 0;
            double sumProductsY = 0;

            std::vector<double> xValues = this->stochastValues[x];
            std::vector<double> yValues = this->stochastValues[y];

            for (size_t i = 0; i < std::min(xValues.size(), yValues.size()); i++)
            {
                double weight = weights.empty() ? 1.0 : weights[i];

                sumCross += weight * xValues[i] * yValues[i];
                sumProductsX += weight * xValues[i] * xValues[i];
                sumProductsY += weight * yValues[i] * yValues[i];
            }

            double correlationCoefficient = sumCross / std::sqrt(sumProductsX * sumProductsY);

            return correlationCoefficient;
        }

        std::shared_ptr<Statistics::CorrelationMatrix> CorrelationMatrixBuilder::getCorrelationMatrix()
        {
            std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = std::make_shared<Statistics::CorrelationMatrix>();

            correlationMatrix->init(this->stochasts);

            for (size_t i = 0; i < stochasts.size(); i++)
            {
                for (size_t j = 0; j < i; j++)
                {
                    double correlationValue = this->getCorrelationValue(stochasts[i], stochasts[j]);
                    correlationMatrix->SetCorrelation(stochasts[i], stochasts[j], correlationValue);
                }
            }

            return correlationMatrix;
        }
    }
}

