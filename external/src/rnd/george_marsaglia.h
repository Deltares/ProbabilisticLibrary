#pragma once
#include <random>
#include "randomForSampling.h"

namespace Deltares {
    namespace ProbLibCore {

        struct gm_state
        {
            double  u[97 + 1];
            double  c;
            double  cd;
            double  cm;
            int     i97;
            int     j97;
        };

        class george_marsaglia : public randomForSampling
        {
        public:
            george_marsaglia();
            george_marsaglia(const int ij, const int kl);
            virtual double get_uniform();
            //virtual double get_std_normal();
            virtual void restart();
        private:
            void init(const int ij, const int kl);
            gm_state state;
            const int s1 = 0, s2 = 0;
        };
    }
}
