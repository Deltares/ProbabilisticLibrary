#include "GradientCalculator.h"

#include <vector>

#include "GradientSettings.h"
#include "Sample.h"
#include "ZModelRunner.h"

namespace Deltares
{
    namespace Models
    {
        std::vector<double> GradientCalculator::getGradient(Models::ZModelRunner* modelRunner, Sample* sample)
        {
            int nstochasts = modelRunner->getVaryingStochastCount();

            std::vector<Sample*> samples;
            std::vector<double> gradient(nstochasts);

            // first sample is the sample itself
            samples.push_back(sample);

            if (Settings->gradientType == OneDirection)
            {
                double du = Settings->StepSize * 0.5;
                for (int k = 0; k < nstochasts; k++)
                {
                    Sample* uNew = sample->clone();
                    uNew->Values[k] += du;

                    samples.push_back(uNew);
                }

                double* zValues = modelRunner->getZValues(samples);

                double z = zValues[0];
                for (int k = 0; k < nstochasts; k++)
                {
                    double zp = zValues[k + 1];
                    gradient[k] = (zp - z) / du;
                }

                delete[] zValues;
            }
            else
            {
                for (int k = 0; k < nstochasts; k++)
                {
                    Sample* u1 = sample->clone();
                    u1->Values[k] -= Settings->StepSize * 0.5;
                    samples.push_back(u1);

                    Sample* u2 = sample->clone();
                    u2->Values[k] += Settings->StepSize * 0.5;
                    samples.push_back(u2);
                }

                double* zValues = modelRunner->getZValues(samples);

                for (int k = 0; k < nstochasts; k++)
                {
                    double zm = zValues[2 * k + 1];
                    double zp = zValues[2 * k + 2];
                    gradient[k] = (zp - zm) / Settings->StepSize;
                }

                delete[] zValues;
            }

            for (size_t i = 1; i < samples.size(); i++)
            {
                delete samples[i];
            }

            return gradient;
        }
    }
}
