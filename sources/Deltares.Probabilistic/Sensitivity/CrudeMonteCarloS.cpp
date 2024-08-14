#include "CrudeMonteCarloS.h"
#include <vector>
#include <cmath>
#include <memory>

#include "../Model/Sample.h"
#include "../Model/RandomSampleGenerator.h"
#include "CrudeMonteCarloSettingsS.h"

using namespace Deltares::Models;

namespace Deltares
{
    namespace Sensitivity
    {
        std::shared_ptr<Statistics::Stochast> CrudeMonteCarloS::getStochast(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            std::shared_ptr<SampleProvider> sampleProvider = std::make_shared<SampleProvider>(this->Settings->StochastSet, false);
            modelRunner->setSampleProvider(sampleProvider);

            const std::shared_ptr<RandomSampleGenerator> randomSampleGenerator = std::make_shared<RandomSampleGenerator>();
            randomSampleGenerator->Settings = this->Settings->randomSettings;
            randomSampleGenerator->Settings->StochastSet = this->Settings->StochastSet;
            randomSampleGenerator->sampleProvider = sampleProvider;
            randomSampleGenerator->initialize();

            int nParameters = modelRunner->getVaryingStochastCount();
            std::vector<double> zValues; // copy of z for all parallel threads as double

            std::shared_ptr<Sample> uMin = std::make_shared<Sample>(nParameters);
            std::vector<std::shared_ptr<Sample>> samples;
            std::vector<double> zSamples;
            std::vector<double> zWeights;
            size_t zIndex = 0;
            int nSamples = 0;

            int requiredSamples = this->Settings->Samples;
            if (this->Settings->DeriveSamplesFromVariationCoefficient)
            {
                requiredSamples = this->Settings->getRequiredSamples();
            }

            for (int sampleIndex = 0; sampleIndex < requiredSamples && !isStopped(); sampleIndex++)
            {
                zIndex++;

                if (zIndex >= samples.size())
                {
                    samples.clear();

                    int chunkSize = modelRunner->Settings->MaxChunkSize;
                    int runs = std::min(chunkSize, Settings->Samples - sampleIndex);

                    sampleProvider->reset();

                    for (int i = 0; i < runs; i++)
                    {
                        std::shared_ptr<Sample> sample = randomSampleGenerator->getRandomSample();
                        samples.push_back(sample);
                    }

                    zValues = modelRunner->getZValues(samples);

                    zIndex = 0;
                }

                double z = zValues[zIndex];
                std::shared_ptr<Sample> u = samples[zIndex];

                if (std::isnan(z))
                {
                    continue;
                }

                zSamples.push_back(z);
                zWeights.push_back(1.0);

                nSamples++;
            }

            std::shared_ptr<Statistics::Stochast> stochast = this->getStochastFromSamples(zSamples, zWeights);

            return stochast;
        }
    }
}



