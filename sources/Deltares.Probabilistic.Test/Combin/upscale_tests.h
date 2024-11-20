// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

namespace Deltares::Probabilistic::Test
{
    class upscale_tests
    {
    public:
        static void runAll();
    private:
        static void upscaleInTimeTests1();
        static void upscaleInTimeTests2();
        static void upscaleInTimeTests3();
        static void upscaleInTimeTests4();
        static void upscaleInTimeTests5();
        static void upscaleInTimeTests6();
        static void upscaleInTimeTests7();
        static void upscaleInTimeTests8();
        static void EquivalentAlphaTesting1();
        static void EquivalentAlphaTesting2();
        static void EquivalentAlphaTesting3();
        static void EquivalentAlphaTesting4();
        static void EquivalentAlphaTesting5();
        static void EquivalentAlphaTesting6();
        static void upscaleToLargestBlockTests1();
        static void upscaleToLargestBlockTests2();
        static void upscaleToLargestBlockTests3();
    };
}
