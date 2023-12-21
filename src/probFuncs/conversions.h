#pragma once

namespace Deltares {
    namespace ProbLibCore {

        class conversions
        {
        public:
            static double betaFromQ(const double q);
            static double betaFromP(const double p);
            static void PQfromBeta(const double u, double& p, double& q);
            static double PfromBeta(const double u);
            static double QfromBeta(const double u);
            static double Dgammq(const double x, const double p);
            static double ReturnTimeFromBeta(const double beta);
            static double FreqFromBeta(const double beta);
            static double LogQFromBeta(const double beta);
        private:
            static constexpr double qMin = 1.0e-300;
            static constexpr double uLimit = 5.6;
            static void normp(const double z, double& p, double& q, double& pdf);
            static double betaFromQ_cdf_inverse(const double q);
        };
    }
}

#ifdef __cplusplus_cli
#include <iostream>
namespace Deltares {
    namespace ProbLib {
#pragma managed
        public delegate int MyDel(int i);
        public delegate double MyZdel(intptr_t x);

        public ref class ab
        {
        public:
            ab(const double aa, const double bb) : a(aa), b(bb) {}
            double sum() { return a + b; }
            const double a;
            const double b;
        };

        public ref class ierr
        {
        public:
            ierr(const int aa, char bb[]) : a(aa) {}
            int a;
            char b;
        };

        public ref class funcDel
        {
        public:
            int func1(int i) { std::cout << i; return 0; }
            int func2(int i) { System::Console::WriteLine(i); return 0; }
            int func3(MyDel^ f, int i) { return f(i); }
            double func4(MyZdel^ f, intptr_t x)
            {
                auto z = f(x);
                return z;
            }
        };

        public ref class conversions2
        {
        public:
            static double betaFromQ(const double q)
            {
                return Deltares::ProbLibCore::conversions::betaFromQ(q);
            }
        };
    }
}
#endif

