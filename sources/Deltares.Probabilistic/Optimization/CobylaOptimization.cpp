// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "CobylaOptimization.h"
#include "Cobyla.h"
#include <functional>

namespace Deltares::Optimization
{
    OptimizationSample CobylaOptimization::GetCalibrationPoint(const SearchArea& searchArea, std::shared_ptr<optimizationModel> model) const
    {
        unsigned n = static_cast<unsigned>(searchArea.Dimensions.size());
        unsigned m = model->GetNumberOfConstraints();

        auto x0 = std::vector<double>(n);
        auto lb = std::vector<double>(n);
        auto ub = std::vector<double>(n);
        auto dx = std::vector<double>(n);
        for (unsigned i = 0 ; i < n; i++)
        {
            x0[i] = searchArea.Dimensions[i].StartValue;
            lb[i] = searchArea.Dimensions[i].LowerBound;
            ub[i] = searchArea.Dimensions[i].UpperBound;
            dx[i] = 0.1;
        }
        long long fData;
        void* funcData = &fData;

        auto myfunc = [model](unsigned n, const double* x, [[maybe_unused]] double* gradient, [[maybe_unused]] void* func_data)
        {
            auto s = std::make_shared<Models::Sample>(n);
            for (unsigned i = 0; i < n; i++)
            {
                s->Values[i] = x[i];
            }
            return model->GetZValue(s);
        };

        auto myfuncC = [model](unsigned n, const double* x, [[maybe_unused]] double* gradient, [[maybe_unused]] void* func_data)
            {
                auto s = std::make_shared<Models::Sample>(n);
                for (unsigned i = 0; i < n; i++)
                {
                    s->Values[i] = x[i];
                }
                return model->GetConstraintValue(s);
            };

        auto fc = std::vector<nlopt_constraint>(m);
        if (m > 0)
        {
            fc[0].f = myfuncC;
            fc[0].m = 1;
            fc[0].tol = std::vector<double>(1);
            fc[0].tol[0] = settings.EpsilonBeta;
        }
        auto h = std::vector<nlopt_constraint>(0);
        auto minf = std::vector<double>(1);
        nlopt_stopping stop = nlopt_stopping();
        auto nevals = std::vector<int>(1);
        stop.nevals_p = nevals.data();
        stop.xtol_rel = settings.EpsilonBeta;
        stop.maxeval = settings.MaxIterations;
        unsigned p = 0;

        auto status = cobyla_minimize(n, myfunc, funcData, m, fc.data(), p, h.data(),
            lb.data(), ub.data(), x0.data(), minf.data(), &stop, dx.data());

        OptimizationSample s;
        s.numberOfSamples = *stop.nevals_p;
        s.minimumValue = minf[0];
        switch (status)
        {
        case NLOPT_SUCCESS:
        case NLOPT_STOPVAL_REACHED:
        case NLOPT_FTOL_REACHED:
        case NLOPT_XTOL_REACHED:
            s.success = true;
            break;
        default:
            s.success = false;
            break;
        }

        for (unsigned i = 0; i < n; i++)
        {
            s.Input.push_back(x0[i]);
        }
        return s;
    };
}

