#include "DesignPointModel.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Reliability
    {
        void DesignPointModel::calculate(std::shared_ptr<ModelSample> sample)
        {
            // construct the tangent line of the circle with radius beta and in coordinates (- alfa1 * beta, -alfa2 * beta, ...)
            // then detect whether on which side (x1, x2, .. ) is located

            // for all points on the tangent line, the following is true:
            // sum (-alfa_i * x_i) = beta, since x_i = = - alfa_i * beta
            // so  beta - sum (-alfa_i * x_i) = 0
            // so  beta + sum (alfa_i * x_i) = 0

            // if all x are zero, then we expect a z_value of beta

            // check:
            // if alfa is positive, we expect a higher value for z with increasing x (strength variable)
            // if alfa is negative, we expect a lower value for z with increasing x (load variable)

            double result = this->designPoint->Beta;

            for (size_t i = 0; i < sample->Values.size(); i++)
            {
                result += sample->Values[i] * alphas[i];
            }

            if (this->inverted && this->negated)
            {
                result = result > 0 ? nan("") : -result;
            }
            else if (this->negated)
            {
                result = -result;
            }
            else if (this->inverted)
            {
                result = result > 0 ? nan("") : result;
            }

            sample->Z = result;
        }

        double DesignPointModel::getDirectionBeta(std::shared_ptr<ModelSample> sample)
        {
            this->calculate(sample);

            return Numeric::NumericSupport::interpolate(0, this->designPoint->Beta, 0,  sample->Z, 1, true);
        }

        void DesignPointModel::setParameters(std::vector<std::shared_ptr<Statistics::Stochast>>& parameters)
        {
            alphas.clear();

            for (size_t i = 0; i < parameters.size(); i++)
            {
                double alphaValue = 0;

                for (size_t j = 0; j < this->designPoint->Alphas.size(); j++)
                {
                    if (this->designPoint->Alphas[j]->Stochast == parameters[i])
                    {
                        alphaValue = this->designPoint->Alphas[j]->Alpha;
                        break;
                    }
                }

                alphas.push_back(alphaValue);
            }
        }
    }
}

