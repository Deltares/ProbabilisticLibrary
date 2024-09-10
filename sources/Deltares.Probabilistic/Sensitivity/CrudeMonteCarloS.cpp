#include "CrudeMonteCarloS.h"
#include <vector>
#include <cmath>
#include <memory>

#include "../Math/NumericSupport.h"
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

            std::vector<double> zValues; // copy of z for all parallel threads as double

            std::vector<std::shared_ptr<Sample>> samples;
            std::vector<double> zSamples;
            size_t zIndex = 0;
            int nSamples = 0;

            bool registerSamplesForCorrelation = this->correlationMatrixBuilder->isEmpty() && this->Settings->CalculateCorrelations && this->Settings->CalculateInputCorrelations;

            int requiredSamples = std::min(this->Settings->getRequiredSamples(), this->Settings->MaximumSamples);

            for (int sampleIndex = 0; sampleIndex < requiredSamples && !isStopped(); sampleIndex++)
            {
                zIndex++;

                if (zIndex >= samples.size())
                {
                    samples.clear();

                    int chunkSize = modelRunner->Settings->MaxChunkSize;
                    int runs = std::min(chunkSize, Settings->MaximumSamples - sampleIndex);

                    sampleProvider->reset();

                    for (int i = 0; i < runs; i++)
                    {
                        std::shared_ptr<Sample> sample = randomSampleGenerator->getRandomSample();
                        samples.push_back(sample);
                    }

                    modelRunner->getZValues(samples);

                    zIndex = 0;
                }

                double z = samples[zIndex]->Z;

                if (std::isnan(z))
                {
                    continue;
                }

                zSamples.push_back(z);

                if (registerSamplesForCorrelation)
                {
                    modelRunner->registerSample(this->correlationMatrixBuilder, samples[zIndex]);
                }

                nSamples++;
            }

            std::vector<double> zWeights = Numeric::NumericSupport::select(zSamples, [](double x) {return 1.0; });

            std::shared_ptr<Statistics::Stochast> stochast = this->getStochastFromSamples(zSamples, zWeights);

            if (this->Settings->CalculateCorrelations)
            {
                this->correlationMatrixBuilder->registerSamples(stochast, zSamples);
            }

            return stochast;
        }
    }
}



