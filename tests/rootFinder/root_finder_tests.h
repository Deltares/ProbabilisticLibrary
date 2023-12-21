#pragma once
#include "../../src/rootFinders/root_finder.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        class sampleFunc1 : public FuncWrapper
        {
        public:
            virtual double GetValue(const double x)
            {
                lastZ = x - 3.0;
                counter++;
                return lastZ;
            };
            void getStats(double& y, size_t& i) { y = lastZ; i = counter; };
        private:
            double lastZ;
            size_t counter;
        };

        class sampleFunc2 : public FuncWrapper
        {
        public:
            virtual double GetValue(const double x)
            {
                lastZ = x * x - 3.0;
                counter++;
                return lastZ;
            };
            void getStats(double& y, size_t& i) { y = lastZ; i = counter; };
        private:
            double lastZ;
            size_t counter;
        };

        class nodFunc : public FuncWrapper
        {
        public:
            virtual double GetValue(const double x)
            {
                lastZ = (x > 0.0 ? x : 2.0 * x) - 4.0;
                counter++;
                return lastZ;
            }
            void getStats(double& y, size_t& i) { y = lastZ; i = counter; };
        private:
            double lastZ;
            size_t counter;
        };

        class root_finder_tests
        {
        public:
            void all_root_finder_tests();
        private:
            void linear_loop_test1();
            void linear_loop_test2();
            void linear_loop_test3();
            void linear_loop_test4();
            void linear_loop_test5();
            void linear_loop_test_par();
            void bisection_test1();
            void regula_falsi_test1();
            void regula_falsi_test2();
            void regula_falsi_test3();
            void regula_falsi_test4();
            void anderson_bjork_test1();
            void anderson_bjork_nod_test();
            void anderson_bjork_outside_x0_x1_test();
            void dsRootFinder_test1();
            void dsRootFinder_test2a();
            void dsRootFinder_test2b();
            void dsRootFinder_test3();
            void dsRootFinder_testNod();
            void dsRootFinder_testNodRobust();
            void dsRootFinder_testNodRobustBisection();
        };
    }
}
