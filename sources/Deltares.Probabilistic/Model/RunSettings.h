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

#include "Message.h"
#include "ProxySettings.h"

namespace Deltares
{
    namespace Models
    {
        enum class designPointOptions
        {
            dpOutFALSE = 0,
            dpOutTRUE = 1,
            dpOutPrintAll = 3,
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
            MessageType LowestMessageType = MessageType::Warning;
            bool UseOpenMPinReliability = true; // false: parallelization only using getZValues; needed for Python

            std::shared_ptr<ProxySettings> proxySettings = nullptr;

            bool isValid()
            {
                return MaxParallelProcesses >= 1;
            }

            bool IsProxyModel()
            {
                bool isProxyModel = false;
                if (proxySettings.get() != nullptr)
                {
                    isProxyModel = proxySettings->IsProxyModel;
                }
                return isProxyModel;
            }
        };
    }
}

