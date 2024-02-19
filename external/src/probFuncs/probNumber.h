#pragma once

namespace Deltares {
    namespace ProbLibCore {

        class probNumber
        {
        public:
            probNumber();
            probNumber(const double beta);
            probNumber(const double pq, const char f);
            double getu();
            double getp();
            double getq();
        private:
            double u;
            double p;
            double q;
            bool isdirtyU;
            bool isdirtyPQ;
            char i; // copy of f: p/q is initialized with p or q
        };
    }
}
