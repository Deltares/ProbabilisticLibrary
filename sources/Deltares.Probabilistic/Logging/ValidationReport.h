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
#include <vector>
#include "Message.h"

namespace Deltares
{
    namespace Logging
    {
        /**
         * \brief Contains the results of validation of a certain object
         */
        class ValidationReport
        {
        public:
            ValidationReport() = default;

            /**
             * \brief Validation messages
             */
            std::vector<std::shared_ptr<Message>> messages;

            /**
             * \brief Indicates whether the validated object is valid
             * \return Indication
             */
            bool isValid() const
            {
                for (const std::shared_ptr<Message>& message : messages)
                {
                    if (message->Type == Error)
                    {
                        return false;
                    }
                }

                return true;
            }
        };

    }
}

