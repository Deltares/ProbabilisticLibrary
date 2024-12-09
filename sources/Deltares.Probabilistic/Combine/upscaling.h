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
#include "../Math/vector1D.h"
#include "alphaBeta.h"
#include "HohenbichlerFORM.h"

namespace Deltares {
    namespace Reliability {

        //
        // Class to combine failure probabilities for identical time elements (identical = same reliability index and alpha values)
        //
        class upscaling
        {
        public:
            int upscaleInTime(const double nrTimes, alphaBeta& element, const vector1D& inRhoT);
            std::pair<alphaBeta, int> upscaleLength(alphaBeta& crossSectionElement, const vector1D& rhoXK,
                const vector1D& dXK, const double sectionLength);
            void upscaleToLargestBlock(const alphaBeta& smallBlock,
                const vector1D& rhoTSmallBlock, const vector1D& blockDurations, const double largestBlockDuration,
                alphaBeta& largestBlock, vector1D& durationsLargestBlock);
            // TODO: ComputeBetaSection is public for testing
            std::pair<double, int> ComputeBetaSection(const double betaCrossSection, const double sectionLength,
                const double rhoZ, const double dz, const double deltaL);
        private:
            const double rhoLimit = 0.99999; // Limit value for the correlation coefficient
            const double rhoLowLim = 0.001;          // Lower limit of correlation coefficient before Hohenbichler method must be called
            HohenbichlerFORM hhb = HohenbichlerFORM();
            double upscaleBeta(double elm, const double rhoT, const double nrTimes, int& failures);
        };
    }
}
