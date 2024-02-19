#include "randomForSampling.h"
#include "../probFuncs/conversions.h"

namespace Deltares {
    namespace ProbLibCore {

        double randomForSampling::get_std_normal()
        {
            double u = get_uniform();
            double r = conversions::betaFromQ(u);
            return r;
        }

        void randomForSampling::getRandomVector(vector1D& r)
        {
            auto n = r.size();
            for (size_t i = 0; i < n; i++)
            {
                r(i) = get_std_normal();
            }
        }

        void randomForSampling::getNormalizedRandomVector(vector1D& r)
        {
            getRandomVector(r);
            r.normalize();
        }
    }
}
