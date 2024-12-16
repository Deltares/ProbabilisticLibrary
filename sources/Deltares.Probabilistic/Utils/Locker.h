// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
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

#include <mutex>

namespace Deltares
{
    namespace Utils
    {
        /**
         * \brief Facilitates locking of a thread to avoid synchronization problems
         * \remark By implementing this in a separate class, problems are avoid when mutex were defined in a class which is used in a shared pointer
         */
        class Locker
        {
        public:
            /**
             * \brief Ensures that the code until unlock() is executed by one thread at a time
             */
            void lock() { mutexObject.lock(); }

            /**
             * \brief Ends the code protected by lock()
             */
            void unlock() { mutexObject.unlock(); }
        private:
            std::mutex mutexObject;
        };
    }
}

