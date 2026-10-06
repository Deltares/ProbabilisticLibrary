// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "CobylaOptimization.h"
#include "Cobyla.h"
#include <functional>

namespace Deltares::Numeric
{
    class NumericSupport;
}

namespace Deltares::Optimization
{
    std::shared_ptr<OptimizationResult> CobylaOptimization::getOptimizedSample(Models::ZModel& model)
    {
        auto searchArea = Settings.SearchArea;

        const unsigned n = static_cast<unsigned>(searchArea->Dimensions.size());
        const unsigned m = model.hasConstraint() ? 1 : 0; // model.GetNumberOfConstraints();

        auto x0 = std::vector<double>(n);
        auto lb = std::vector<double>(n);
        auto ub = std::vector<double>(n);
        auto dx = std::vector<double>(n);
        for (unsigned i = 0 ; i < n; i++)
        {
            x0[i] = searchArea->Dimensions[i]->StartValue;
            lb[i] = searchArea->Dimensions[i]->MinValue;
            ub[i] = searchArea->Dimensions[i]->MaxValue;
            dx[i] = 0.1;
        }
        long long fData = 0;

        auto myfunc = [&model](unsigned dim_x, const double* x, [[maybe_unused]] double* gradient, [[maybe_unused]] void* func_data)
        {
            auto s = Models::ModelSample(static_cast<int>(dim_x));
            for (unsigned i = 0; i < dim_x; i++)
            {
                s.Values[i] = x[i];
            }

            // get value to minimize

            model.invoke(s);

            return s.Z;
        };

        auto myfuncC = [&model](unsigned dim_x, const double* x, [[maybe_unused]] double* gradient, [[maybe_unused]] void* func_data)
            {
            auto s = Models::ModelSample(static_cast<int>(dim_x));
                for (unsigned i = 0; i < dim_x; i++)
                {
                    s.Values[i] = x[i];
                }

                // get z value
                double constraint = model.getConstraint(s);

                return constraint;

            };

        auto fc = std::vector<nlopt_constraint>(m);
        if (m > 0)
        {
            fc[0].f = myfuncC;
            fc[0].m = 1;
            fc[0].tol = std::vector<double>(1);
            fc[0].tol[0] = Settings.EpsilonBeta;
        }
        auto h = std::vector<nlopt_constraint>(0);
        double minimum_f_value = 0.0;
        auto stop = nlopt_stopping();
        int number_of_evaluations = 0;
        stop.nevals_p = &number_of_evaluations;
        stop.xtol_rel = Settings.EpsilonBeta;
        stop.maxeval = Settings.MaxIterations;
        unsigned p = 0;

        auto status = cobyla_minimize(n, myfunc, &fData, m, fc.data(), p, h.data(),
            lb.data(), ub.data(), x0.data(), &minimum_f_value, &stop, dx.data());

        auto result = std::make_shared<OptimizationResult>();
        result->modelRuns = *stop.nevals_p;
        result->minimumValue = minimum_f_value;
        switch (status)
        {
        case NLOPT_SUCCESS:
        case NLOPT_STOPVAL_REACHED:
        case NLOPT_FTOL_REACHED:
        case NLOPT_XTOL_REACHED:
            result->succeeded = true;
            break;
        default:
            result->succeeded = false;
            break;
        }

        // copy results, do not reuse vector
        // reusing vector leads to memory problems on linux

        result->values.reserve(x0.size());
        for (size_t i = 0; i < x0.size(); i++)
        {
            result->values[i] = x0[i];
        }

        return result;
    };
}

