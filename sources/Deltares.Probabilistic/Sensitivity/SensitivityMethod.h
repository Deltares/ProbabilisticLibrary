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
#include "../Model/ModelRunner.h"

#include <memory>

#include "SensitivityResult.h"

namespace Deltares::Sensitivity
{
    /**
     * \brief Base class for calculation of the sensitivity
     */
    class SensitivityMethod
    {
    public:

        virtual ~SensitivityMethod() = default;

        /**
         * \brief Gets the sensitivity
         * \param modelRunner The model for which the sensitivity is calculated
         * \return The sensitivity 
         */
        virtual SensitivityResult getSensitivityResult(std::shared_ptr<Models::ModelRunner> modelRunner)
            { return SensitivityResult(); }

        /**
         * \brief Indicates whether the calculation has been stopped
         */
        bool isStopped() const;

        /**
         * \brief Stops the calculation
         * \remark Usually called from another thread
         */
        void Stop();

    protected:
        virtual void setStopped();

    private:
        bool stopped = false;
    };
}


