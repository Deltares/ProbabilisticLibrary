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
#pragma once
#include "../../Deltares.Probabilistic/Optimization/CobylaOptimization.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class testModel : public Optimization::optimizationModel
            {
            public:
                testModel() = default;
                testModel(double offset_1, double offset_2) : offset1(offset_1), offset2(offset_2) {}
                double GetZValue(std::shared_ptr<Models::Sample> sample) const override;
            private:
                const double offset1 = -1.0;
                const double offset2 = 0.0;
            };

            class testModelWithConstraint : public Optimization::optimizationModel
            {
            public:
                double GetZValue(std::shared_ptr<Models::Sample> sample) const override;
                double GetConstraintValue(const Models::Sample& sample) const override;
                unsigned GetNumberOfConstraints() const override { return 1; }
            };

            class testCobyla
            {
            public:
                void allCobylaTests();
            private:
                static void test_no_constraints1();
                static void test_no_constraints2();
                static void test_with_constraint1();
                const double margin = 1e-2;
            };
        }
    }
}

