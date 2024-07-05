#include "GradientCalculator.h"

#include <vector>

#include "GradientSettings.h"
#include "Sample.h"
#include "ModelRunner.h"
#include "../Utils/probLibException.h"

using namespace Deltares::Reliability;

namespace Deltares
{
    namespace Models
    {
        std::vector<double> GradientCalculator::getGradient(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> sample)
        {
            int nstochasts = modelRunner->getVaryingStochastCount();

            std::vector<std::shared_ptr<Sample>> samples;
            std::vector<double> gradient(nstochasts);

            // first sample is the sample itself
            samples.push_back(sample);

            if (Settings->gradientType == OneDirection)
            {
                double du = Settings->StepSize * 0.5;
                for (int k = 0; k < nstochasts; k++)
                {
                    std::shared_ptr<Sample> uNew = sample->clone();
                    uNew->Values[k] += du;

                    samples.push_back(uNew);
                }

                std::vector<double> zValues = modelRunner->getZValues(samples);

                double z = zValues[0];
                for (int k = 0; k < nstochasts; k++)
                {
                    double zp = zValues[k + 1];
                    gradient[k] = (zp - z) / du;
                }
            }
            else if (Settings->gradientType == TwoDirections)
            {
                for (int k = 0; k < nstochasts; k++)
                {
                    std::shared_ptr<Sample> u1 = sample->clone();
                    u1->Values[k] -= Settings->StepSize * 0.5;
                    samples.push_back(u1);

                    std::shared_ptr<Sample> u2 = sample->clone();
                    u2->Values[k] += Settings->StepSize * 0.5;
                    samples.push_back(u2);
                }

                std::vector<double> zValues = modelRunner->getZValues(samples);

                for (int k = 0; k < nstochasts; k++)
                {
                    const double zLow = zValues[2 * k + 1];
                    const double zHigh = zValues[2 * k + 2];

                    gradient[k] = (zHigh - zLow) / Settings->StepSize;
                }
            }
            else
            {
                throw probLibException("gradient type");
            }

            return gradient;
        }
    }
}
