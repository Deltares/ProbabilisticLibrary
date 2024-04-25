#pragma once

#include <mutex>

namespace Deltares
{
    namespace Utils
	{
		/**
		 * \brief Facilitates locking of a thread to avoid synchronization problems
		 * \remark By implementing this in a separate thread, problems are avoid when mutex were defined in a class which is used in a shared pointer
		 */
		class Locker
		{
		public:
			/**
			 * \brief Ensures that the code until unlock is executed by one thread at a time
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

