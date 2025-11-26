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

#include "../../Deltares.Probabilistic/Math/matrix.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class matinv_tests
            {
            public:
                void all_matinv_tests() const;
            private:
                void matinv_test1() const;
                void matinv_singular_test() const;
                void positive_definite_tests() const;
                static void qr_tests();

                Deltares::Numeric::Matrix get3x3posDefiniteMatrix() const;
                Deltares::Numeric::Matrix get2x2singularMatrix() const;
                Deltares::Numeric::Matrix get2x2symmetrixMatrix() const;
                Deltares::Numeric::Matrix get16x16Matrix() const;
                const double margin = 1e-12;
            };
        }
    }
}
