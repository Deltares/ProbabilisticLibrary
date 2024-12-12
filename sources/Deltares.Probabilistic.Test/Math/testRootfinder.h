// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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

namespace Deltares::Probabilistic::Test
{
    class rootfinder_tests
    {
    public:
        void all_rootfinder_tests();
    private:
        double testLinearFunc(double x) const;
        double testConstFunc(double x);
        double testNodFunc(double x);
        void bisection_test();
        void bisection_test2();
        void bisection_const_test();
        void bisection_nod_test();
        void linear_root_finder_test();
        void linear_root_finder_test2();
        void linear_root_finder_const_test();
        void linear_root_finder_nod_test();
        double a = 0.0;
        double b = 0.0;
        int cnt = 0;
    };
}

