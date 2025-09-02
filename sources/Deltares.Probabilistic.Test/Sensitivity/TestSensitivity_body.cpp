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
#include "TestSensitivity.h"

#include "../projectBuilder.h"

#include "../../Deltares.Probabilistic/Sensitivity/SensitivityProject.h"
#include "../../Deltares.Probabilistic/Sensitivity/SobolSequence.h"
#include "../../Deltares.Probabilistic/Sensitivity/SobolSequenceProvider.h"

#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{
    void TestSensitivity::allSensitivityTests() const
    {
        testSequence();
        testRandomSequence();
        testRepeatable();
    }

    void TestSensitivity::testSequence() const
    {
        int n = 3;
        int d = 10;
        std::vector<std::vector<double>> reference = getExpectedResults();

        std::vector<Sensitivity::SobolDirection> directions = Sensitivity::SobolDirectionLoader::getDirections(d);

        std::vector<Sensitivity::SobolSequence> sequences;
        for (int i = 0; i < d; i++)
        {
            sequences.push_back(Sensitivity::SobolSequence(directions[i]));
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < d; j++)
            {
                double value = sequences[j].nextDouble();
                ASSERT_NEAR(reference[i][j], value, margin);
            }
        }
    }

    void TestSensitivity::testRandomSequence() const
    {
        int n = 3;
        int d = 10;
        std::vector<std::vector<double>> reference = getExpectedResults();

        Sensitivity::SobolSequenceProvider randomSequence = Sensitivity::SobolSequenceProvider(d);

        for (int i = 0; i < n; i++)
        {
            std::vector<double> value = randomSequence.getSequence();
            for (int j = 0; j < d; j++)
            {
                ASSERT_NEAR(reference[i][j], value[j], margin);
            }
        }
    }

    void TestSensitivity::testRepeatable() const
    {
        int d = 10;
        int nMax = 50;

        Sensitivity::SobolSequenceProvider randomSequence = Sensitivity::SobolSequenceProvider(d);

        std::vector<std::vector<double>> sequences;
        for (int n = 0; n < nMax; n++)
        {
            sequences.push_back(randomSequence.getSequence());
        }

        Sensitivity::SobolSequenceProvider secondRandomSequence = Sensitivity::SobolSequenceProvider(d);

        std::vector<std::vector<double>> secondSequences;
        for (int n = 0; n < nMax; n++)
        {
            secondSequences.push_back(secondRandomSequence.getSequence());
        }

        for (int n = 0; n < nMax; n++)
        {
            std::vector<double> value = randomSequence.getSequence();
            for (int j = 0; j < d; j++)
            {
                ASSERT_EQ(sequences[n][j], secondSequences[n][j]);
            }
        }
    }

    std::vector<std::vector<double>> TestSensitivity::getExpectedResults() const
    {
        std::vector<std::vector<double>> reference = {
        { 0.25, 0.75, 0.75, 0.75, 0.25, 0.25, 0.75, 0.25, 0.25, 0.25 },
        { 0.75, 0.25, 0.25, 0.25, 0.75, 0.75, 0.25, 0.75, 0.75, 0.75 },
        { 0.875, 0.875, 0.125, 0.375, 0.875, 0.625, 0.875, 0.375, 0.375, 0.125 }
        };

        return reference;
    };
}

