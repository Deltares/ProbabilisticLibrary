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
#include <functional>
#include <memory>
#include <vector>

#include "../Deltares.Probabilistic/Model/ModelSample.h"
#include "../Deltares.Probabilistic/Model/RunSettings.h"
#include "stringHelper.h"

struct computationSettings
{
    Deltares::Models::designPointOptions dpOut;
    int computationId;         // reserved for e.g. wind direction
    int threadId;
    int reliabilityMethodSubStepsCounter;
};

typedef std::function<double(double[], computationSettings*, tError*)> zFuncExtern;

class funcWrapper
{
public:
    funcWrapper(const int id, zFuncExtern func) : compId(id), zfunc(func) { ; }
    void FDelegate(std::shared_ptr<Deltares::Models::ModelSample> s);
    void FDelegateParallel(std::vector<std::shared_ptr<Deltares::Models::ModelSample>> s);
    std::vector<std::string> error_messages;
private:
    const int compId;
    zFuncExtern zfunc;
}
;

