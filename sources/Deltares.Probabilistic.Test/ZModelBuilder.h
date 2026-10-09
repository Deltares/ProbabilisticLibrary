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
#pragma once
#include "../Deltares.Probabilistic/Model/ZModel.h"

namespace Deltares::Probabilistic::Test
{
    class ZmodelWithCenter : public Models::ZModel
    {
    public:
        ZmodelWithCenter(const std::vector<double>& center) : center(center) {}
        void invoke(Models::ModelSample& sample) override;
        const int getEvaluations() const { return evaluations; }
    private:
        std::vector<double> center;
        int evaluations = 0;
    };

    class ZModelBuilder
    {
    public:

        static Models::ZModel getPolynomeModel(double offset1 = -1.0, double offset2 = 0.0);
        static Models::ZModel getConstrainedPolynomeModel();
    };
}

