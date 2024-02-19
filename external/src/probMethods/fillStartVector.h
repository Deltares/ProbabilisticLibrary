#pragma once
#include "ray_search.h"
#include "sphere_search.h"

namespace Deltares {
    namespace ProbLibCore {

        enum class StartMethods
        {
            Zero = 1,
            One,
            GivenVector,
            RaySearch,
            SphereSearch,
            RaySearchVector = 8,
            RaySearchVectorScaled
        };

        class FillStartVector
        {
        public:
            FillStartVector(const std::string& m, const double* s) : method(convert(m)), startVector(s) {};
            FillStartVector(const StartMethods& m, const double* s) : method(m), startVector(s) {};
            vector1D GetU(const zmodel& w, const stochSettings& s, const int numCores) const;
            static StartMethods convert(const std::string& m);
        private:
            const StartMethods method;
            const double* startVector;
        };
    }
}
