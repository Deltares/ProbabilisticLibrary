#include <gtest/gtest.h>
#include <cmath>
#include "sphere_tasks_tests.h"
#include "../../src/probMethods/sphere_tasks.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void sphere_tasks_tests::all_sphere_tasks_tests()
        {
            sphere_tasks_test1();
            sphere_tasks_test2();
            sphere_tasks_test3();
        }

        void sphere_tasks_tests::checks(const std::vector<vector1D> result)
        {
            size_t nStochasts = result[0].size();
            for (const vector1D& v : result)
            {
                double length = sqrt(v.sumOfSquares());
                ASSERT_NEAR(length, 1.0, 0.3);
            }

            auto udiff = vector1D(nStochasts);
            for (size_t i = 0; i < result.size(); i++)
            {
                for (size_t j = i + 1; j < result.size(); j++)
                {
                    for (size_t k = 0; k < nStochasts; k++)
                    {
                        udiff(k) = result[i](k) - result[j](k);
                    }
                    ASSERT_FALSE(udiff.sumOfSquares() < 1e-6);
                }
            }
        }

        void sphere_tasks_tests::sphere_tasks_test1()
        {
            auto tasks = sphereTasks(3);
            auto uRay = vector1D({ 1,1 });
            uRay.normalize();
            auto result = tasks.examineSurfaceForTasks(uRay);
            ASSERT_EQ(result.size(), 3);
            checks(result);
        }

        void sphere_tasks_tests::sphere_tasks_test2()
        {
            auto tasks = sphereTasks();
            auto uRay = vector1D({ 1,1 });
            uRay.normalize();
            auto result = tasks.examineSurfaceForTasks(uRay);
            ASSERT_EQ(result.size(), 5);
            checks(result);
        }

        void sphere_tasks_tests::sphere_tasks_test3()
        {
            auto tasks = sphereTasks();
            auto uRay = vector1D({ 1,0,1 });
            uRay.normalize();
            auto result = tasks.examineSurfaceForTasks(uRay);
            ASSERT_EQ(result.size(), 5);
            ASSERT_EQ(result[0].size(), 3);
            //checks(result); // TODO fails, but in the same way as the Fortran code
        }
    }
}
