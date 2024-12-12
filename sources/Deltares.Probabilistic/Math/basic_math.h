// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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

namespace Deltares {
    namespace Reliability {
        // T in max can be int, float and (long) double
        template <typename T>
        inline T max(const T a, const T b)
        {
            return (a > b ? a : b);
        }

        // T in min can be int, float and (long) double
        template <typename T>
        inline T min(const T a, const T b)
        {
            return (a < b ? a : b);
        }

        // T in interpLine can be float or (long) double
        // note that if x1 == x2, interpLine returns NaN.
        template <typename T>
        inline T interpLine(const T x, const T x1, const T x2, const T y1, const T y2)
        {
            T w = (x - x1) / (x2 - x1);
            const T one = (T)1.0;
            T interp = w * y2 + (one - w) * y1;
            return interp;
        }

        // T in sign can be int, float and (long) double
        template <typename T>
        inline T sign(const T a, const T b)
        {
            return (b >= 0 ? std::abs(a) : -std::abs(a));
        }

        // T in mod can be int, size_t etc.
        template <typename T>
        inline T mod(T a, T b)
        {
            return a % b;
        }
    }
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
