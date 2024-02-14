#pragma once

#include <memory>
#include <vector>

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
				std::shared_ptr<T> sharedPointer = nullptr;

				static std::vector<std::shared_ptr<T>> inline sharedPointers;
			public:
				std::shared_ptr<T> getSharedPointer(T* object)
				{
					if (sharedPointer == nullptr)
					{
						sharedPointer = std::make_shared<T>(*object);
						sharedPointers.push_back(sharedPointer);
					}

					return sharedPointer;
				}

				bool isInitialized()
				{
					return sharedPointer != nullptr;
				}

				std::shared_ptr<T> getSharedPointer()
				{
					return sharedPointer;
				}

				void setSharedPointer(std::shared_ptr<T> object)
				{
					sharedPointer = object;
				}

				void setSharedPointer(T* object)
				{
					sharedPointer = std::make_shared<T>(*object);
					sharedPointers.push_back(sharedPointer);
				}

				~SharedPointerProvider()
				{
					int k = 1;
				}
			};
		}
	}
}

