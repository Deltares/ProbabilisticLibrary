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

#include <cmath>
#include <vector>
#include <memory>

namespace Deltares::Models
 {
     class ModelSample
     {
     public:
         ModelSample(const std::vector<double>& values)
         {
             Values = values;
         }

         void clear();

         void copyFrom(const std::shared_ptr<ModelSample>& source);

         bool hasSameValues(const ModelSample& other) const;

         std::vector<double> Values;
         std::vector<double> OutputValues;

         int IterationIndex = -1;
         int threadId = 0;
         double Weight = 1;
         bool AllowProxy = true;
         bool IsRestartRequired = false;
         double Beta = 0;
         double Z = nan("");

         bool ExtendedLogging = false;

         /**
          * Indication of a logging message
          */
         int LoggingCounter = 0;

         int Tag = 0;

     };
}

