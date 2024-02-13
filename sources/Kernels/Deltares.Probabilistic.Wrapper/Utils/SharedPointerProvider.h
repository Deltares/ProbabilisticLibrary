#pragma once

#include <memory>

namespace Deltares
{
	namespace Utils
	{
		namespace Wrappers
		{
			template <class T>

			class SharedPointerProvider
			{
			private:
				bool initialized = false;
				std::shared_ptr<T> sharedPointer;
			public:
				std::shared_ptr<T> getSharedPointer(T* object)
				{
					if (!initialized)
					{
						initialized = true;
						sharedPointer = std::make_shared<T>(*object);
					}

					return sharedPointer;
				}

				void setSharedPointer(std::shared_ptr<T> object)
				{
					sharedPointer = object;
					initialized = true;
				}

				void setSharedPointer(T* object)
				{
					sharedPointer = std::make_shared<T>(*object);
					initialized = true;
				}
			};
		}
	}
}

