#pragma once

#include <memory>

#include "../../Deltares.Probabilistic.Kernels/Model/Message.h"
#include "../Utils/NativeSupport.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public enum class MessageType
			{
				Debug,
				Info,
				Warning,
				Error
			};

			public ref class MessageWrapper
			{
				Kernels::MessageType getMessageType(Deltares::Models::MessageType messageType);
			public:
				MessageWrapper() {}
				MessageWrapper(std::shared_ptr<Models::Message> message)
				{
					this->Type = getMessageType(message->Type);
					this->Text = NativeSupport::toManaged(message->Text);
				}

				property Kernels::MessageType Type;
				property System::String^ Text;
			};
		}
	}
}

