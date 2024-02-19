#pragma once
#include "../../src/utils/vector1D.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {
        class sphere_tasks_tests
        {
        public:
            void all_sphere_tasks_tests();
        private:
            void checks(const std::vector<vector1D> result);
            void sphere_tasks_test1();
            void sphere_tasks_test2();
            void sphere_tasks_test3();
        };
    }
}
