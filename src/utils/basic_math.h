#pragma once

namespace Deltares {
    namespace ProbLibCore {
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

#ifdef __cplusplus_cli
#define M_PI 3.14159265358979323846
#endif
