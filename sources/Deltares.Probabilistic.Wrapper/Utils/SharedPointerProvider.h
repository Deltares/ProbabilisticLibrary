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

			/// <summary>
			/// Holds a smart pointer, which is not possible in a ref class
			/// </summary>
			class SharedPointerProvider
			{
			public:

				SharedPointerProvider() {}

				SharedPointerProvider(T* object)
				{
					this->object = std::make_shared<T>(*object);
				}

				SharedPointerProvider(std::shared_ptr<T> object)
				{
					this->object = object;
				}

				/**
				 * \brief Smart pointer, intended as read only object
				 */
				std::shared_ptr<T> object = nullptr;
			};
		}
	}
}

