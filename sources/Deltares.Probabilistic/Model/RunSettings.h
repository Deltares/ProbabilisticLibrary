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
#include <memory>

#include "../Logging/ValidationReport.h"
#include "../Logging/ValidationSupport.h"

namespace Deltares::Models
{
    enum class designPointOptions
    {
        dpOutFALSE = 0,
        dpOutTRUE = 1,
        dpOutPrintAll = 3,
    };

    enum class HandleInvalidType
    {
        Ignore,
        Fail,
        NoFail
    };

    class RunSettings
    {
    public:
        int MaxParallelProcesses = 1;
        int MaxChunkSize = 16;
        bool SaveEvaluations = false;
        bool SaveConvergence = false;
        bool SaveMessages = false;
        bool ReuseCalculations = true;
        bool RunAtDesignPoint = false;
        bool ExtendedLoggingAtDesignPoint = false;
        int MaxMessages = 1000;
        HandleInvalidType HandleInvalidType = HandleInvalidType::Ignore;
        Logging::MessageType LowestMessageType = Logging::MessageType::Warning;
        bool UseOpenMPinReliability = true; // false: parallelization only using getZValues; needed for Python

        void validate(Logging::ValidationReport& report) const
        {
            Logging::ValidationSupport::checkMinimumInt(report, 1, MaxParallelProcesses, "max parallel processes");
        }

        static std::string getHandleInvalidTypeString(Deltares::Models::HandleInvalidType type);
        static Deltares::Models::HandleInvalidType getHandleInvalidType(const std::string& type);
    };
}

