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
#include "../../Deltares.Probabilistic/Combine/UpScaling.h"
#include "../Utils/TestUtils.h"

namespace Deltares::Probabilistic::Test
{
    class UpscaleTests
    {
    public:
        void runAllUpscaleInTimeTests() const;
        void runAllEquivalentAlphaTests() const;
        void runAllUpscaleToLargestBlockTests() const;
    private:
        void upscaleInTimeTests1() const;
        void upscaleInTimeTests2() const;
        void upscaleInTimeTests3() const;
        void upscaleInTimeTests4() const;
        void upscaleInTimeTests5() const;
        void upscaleInTimeTests6() const;
        void upscaleInTimeTests7() const;
        void upscaleInTimeTests8() const;
        void equivalentAlphaTesting1() const;
        void equivalentAlphaTesting2() const;
        void equivalentAlphaTesting3() const;
        void equivalentAlphaTesting4() const;
        void equivalentAlphaTesting5() const;
        void equivalentAlphaTesting6() const;
        void upscaleToLargestBlockTests1() const;
        void upscaleToLargestBlockTests2() const;
        void upscaleToLargestBlockTests3() const;
        void upscaleToLargestBlockTests4() const;
        Reliability::upscaling upscaler = Reliability::upscaling();
        TestUtils test_utilities = TestUtils();
    };

}

