#pragma once

namespace ASA {
    namespace conversions {

        void beta_inc_values(int& n_data, double& a, double& b, double& x, double& fx);
        double betain(double x, double p, double q, double beta, int& ifault);
    }
}
