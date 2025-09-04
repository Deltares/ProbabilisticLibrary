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
#include "Sobol.h"

#include <algorithm>
#include <memory>

#include "SobolSequenceProvider.h"
#include "../Model/Sample.h"
#include "../Statistics/Stochast.h"
#include "../Math/NumericSupport.h"

namespace Deltares::Sensitivity
{
    using namespace Deltares::Models;
    using namespace Deltares::Statistics;
    using namespace Deltares::Numeric;

    SensitivityResult Sobol::getSensitivityStochast(std::shared_ptr<ModelRunner> modelRunner)
    {
        int nStochasts = modelRunner->getVaryingStochastCount();
        int nSamples = Settings->Iterations;
        int nIterations = 2 + nStochasts * 2;
        int iteration = 0;

        SobolSequenceProvider sequenceProvider(2 * nStochasts);

        // step 1

        std::vector<std::shared_ptr<Sample>> samplesA;
        std::vector<std::shared_ptr<Sample>> samplesB;

        for (int i = 0; i < nSamples; i++)
        {
            std::vector<double> sequence = sequenceProvider.getSequence();
            std::vector<double> uSequence = NumericSupport::select(sequence, [](double p)
            {
                return StandardNormal::getUFromP(p);
            });

            std::vector<double> uValuesA = NumericSupport::take(uSequence, 0, nStochasts);
            std::shared_ptr<Sample> sampleA = std::make_shared<Sample>(uValuesA);
            samplesA.push_back(sampleA);

            std::vector<double> uValuesB = NumericSupport::take(uSequence, nStochasts, nStochasts);
            std::shared_ptr<Sample> sampleB = std::make_shared<Sample>(uValuesB);
            samplesB.push_back(sampleB);
        }

        modelRunner->reportProgress(0, nIterations * nSamples);

        std::vector<double> zA = modelRunner->getZValues(samplesA);
        modelRunner->reportProgress(++iteration * nSamples, nIterations * nSamples);

        std::vector<double> zB = modelRunner->getZValues(samplesB);
        modelRunner->reportProgress(++iteration * nSamples, nIterations * nSamples);

        std::vector<double> zC = NumericSupport::combine(zA, zB);

        double zCMean = NumericSupport::getMean(zC);

        // computing disturbances

        std::vector<std::vector<double>> zAB(nStochasts);
        std::vector<std::vector<double>> zBA(nStochasts);

        for (int index = 0; index < nStochasts; index++)
        {
            std::vector< std::shared_ptr<Sample>> samplesAB = getMixedSamples(index, samplesA, samplesB, nSamples);
            std::vector< std::shared_ptr<Sample>> samplesBA = getMixedSamples(index, samplesB, samplesA, nSamples);

            std::vector<double> zABi = modelRunner->getZValues(samplesAB);
            modelRunner->reportProgress(++iteration * nSamples, nIterations * nSamples);

            std::vector<double> zBAi = modelRunner->getZValues(samplesBA);
            modelRunner->reportProgress(++iteration * nSamples, nIterations * nSamples);

            zAB[index] = zABi;
            zBA[index] = zBAi;
        }

        // calculate variance
        double variance = NumericSupport::sum(zC, [](double p) { return p * p; });
        variance /= static_cast<double>(zC.size());
        variance -= zCMean * zCMean;

        SensitivityResult sensitivityStochast = modelRunner->getSensitivityResult();

        // Compute sobol indices

        for (int index = 0; index < nStochasts; index++)
        {
            double firstOrder = 0;
            for (int j = 0; j < zA.size(); j++)
            {
                firstOrder += zA[j] * zBA[index][j] - zA[j] * zB[j];
            }

            firstOrder /= static_cast<double>(zA.size());

            double firstOrderSobolIndex = firstOrder / variance;

            double total = 0;
            for (size_t j = 0; j < zA.size(); j++)
            {
                total += (zA[j] - zAB[index][j]) * (zA[j] - zAB[index][j]);
            }

            total /= 2 * static_cast<double>(zA.size());

            double totalSobolIndex = total / variance;

            std::shared_ptr<SensitivityValue> value = sensitivityStochast.values[index];
            value->firstOrderIndex = firstOrderSobolIndex;
            value->totalIndex = totalSobolIndex;
        }

        modelRunner->reportProgress(nIterations * nSamples, nIterations * nSamples);

        return sensitivityStochast;
    }

    std::vector<std::shared_ptr<Sample>> Sobol::getMixedSamples(int index, std::vector<std::shared_ptr<Models::Sample>> samples1, std::vector<std::shared_ptr<Models::Sample>> samples2, int nSamples)
    {
        std::vector<std::shared_ptr<Sample>> mixedSamples;

        for (int i = 0; i < nSamples; i++)
        {
            std::shared_ptr<Sample> sample = samples1[i]->clone();
            sample->Values[index] = samples2[i]->Values[index];
            sample->IterationIndex = index;

            mixedSamples.push_back(sample);
        }

        return mixedSamples;
    }
}
