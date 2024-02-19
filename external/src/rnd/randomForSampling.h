#pragma once
#include "../utils/vector1D.h"

namespace Deltares {
    namespace ProbLibCore {

        class randomForSampling
        {
        public:
            randomForSampling() {};
            virtual ~randomForSampling() {};
            virtual double get_uniform() { return -999.0; };
            virtual double get_std_normal();
            virtual void getNormalizedRandomVector(vector1D& r);
            virtual void getRandomVector(vector1D& r);
            virtual void restart() {};
        };
    }
}
