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
#include "../Math/vector1D.h"
#include "alphaBeta.h"
#include "HohenbichlerFORM.h"

namespace Deltares::Reliability
{
    /// <summary>
    /// class with result of an upscaling calculation
    /// </summary>
    struct upscalingResult
    {
        alphaBeta design_point;
        int counter = 0;
        std::string message;
    };

    //
    // Class to combine failure probabilities for identical time elements (identical = same reliability index and alpha values)
    //
    class upscaling
    {
    public:
        int upscaleInTime(const double nrTimes, alphaBeta& element, const Numeric::vector1D& inRhoT) const;
        upscalingResult upscaleLength(const alphaBeta& crossSectionElement, const Numeric::vector1D& rhoXK,
            const Numeric::vector1D& dXK, const double section_length) const;
        void upscaleToLargestBlock(const alphaBeta& smallBlock,
            const Numeric::vector1D& rhoTSmallBlock, const Numeric::vector1D& blockDurations, const double largestBlockDuration,
            alphaBeta& largestBlock, Numeric::vector1D& durationsLargestBlock);
    private:
        const double rhoLimit = 0.99999; // Limit value for the correlation coefficient
        const double rhoLowLim = 0.001;  // Lower limit of correlation coefficient before Hohenbichler method must be called
        HohenbichlerFORM hhb = HohenbichlerFORM();
        double upscaleBeta(double elm, const double rhoT, const double nrTimes, int& failures) const;
    };
}
