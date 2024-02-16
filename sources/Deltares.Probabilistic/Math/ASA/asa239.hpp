#pragma once

namespace ASA {
    namespace conversions {
        double alnorm(double x, bool upper);
        void gamma_inc_values(int* n_data, double* a, double* x, double* fx);
        double gammad(double x, double p);
        double r8_min(double x, double y);
        //void timestamp ( );
    }
}
