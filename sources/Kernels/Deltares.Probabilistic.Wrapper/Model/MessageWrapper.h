#pragma once

#include <memory>

#include "../../Deltares.Probabilistic/Model/Message.h"
#include "../Utils/NativeSupport.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
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
				Wrappers::MessageType getMessageType(Deltares::Models::MessageType messageType);
			public:
				MessageWrapper() {}
				MessageWrapper(std::shared_ptr<Models::Message> message)
				{
					this->Type = getMessageType(message->Type);
					this->Text = Utils::Wrappers::NativeSupport::toManaged(message->Text);
				}

				property Wrappers::MessageType Type;
				property System::String^ Text;
			};
		}
	}
}

