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
#include "../../Deltares.Probabilistic/Combine/upscaling.h"
#include "../../Deltares.Probabilistic/Combine/combineElements.h"
#include "../Utils/testutils.h"

namespace Deltares::Probabilistic::Test
{
    class UpscaleTests
    {
    public:
        void RunAllUpscaleInTimeTests();
        void RunAllEquivalentAlphaTests();
        void RunAllUpscaleToLargestBlockTests();
    private:
        void UpscaleInTimeTests1();
        void UpscaleInTimeTests2();
        void UpscaleInTimeTests3();
        void UpscaleInTimeTests4();
        void UpscaleInTimeTests5();
        void UpscaleInTimeTests6();
        void UpscaleInTimeTests7();
        void UpscaleInTimeTests8();
        void EquivalentAlphaTesting1();
        void EquivalentAlphaTesting2();
        void EquivalentAlphaTesting3();
        void EquivalentAlphaTesting4();
        void EquivalentAlphaTesting5();
        void EquivalentAlphaTesting6();
        void UpscaleToLargestBlockTests1();
        void UpscaleToLargestBlockTests2();
        void UpscaleToLargestBlockTests3();
        Reliability::upscaling upscaler = Reliability::upscaling();
        Reliability::combineElements combiner = Reliability::combineElements();
        testutils test_utilities = testutils();
    };

}

