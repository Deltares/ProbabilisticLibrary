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

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class upscale_tests
            {
            public:
                void runAll();
            private:
                void upscaleInTimeTests1();
                void upscaleInTimeTests2();
                void upscaleInTimeTests3();
                void upscaleInTimeTests4();
                void upscaleInTimeTests5();
                void upscaleInTimeTests6();
                void upscaleInTimeTests7();
                void upscaleInTimeTests8();
                void EquivalentAlphaTesting1();
                void EquivalentAlphaTesting2();
                void EquivalentAlphaTesting3();
                void EquivalentAlphaTesting4();
                void EquivalentAlphaTesting5();
                void EquivalentAlphaTesting6();
                void upscaleToLargestBlockTests1();
                void upscaleToLargestBlockTests2();
                void upscaleToLargestBlockTests3();
                Deltares::Reliability::upscaling up;
                Deltares::Reliability::combineElements cmb;
                testutils ut;
            };

        }
    }
}
