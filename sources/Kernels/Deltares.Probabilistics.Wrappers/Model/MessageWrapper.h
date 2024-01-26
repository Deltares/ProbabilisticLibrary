#pragma once

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
			private:
				Models::Message* message;

			public:
				MessageWrapper() { message = new Models::Message(Models::Info, NativeSupport::toNative("")); }
				MessageWrapper(Models::Message* message) { this->message = message; }
				~MessageWrapper() { this->!MessageWrapper(); }
				!MessageWrapper() { delete message; }

				property Kernels::MessageType Type
				{
					Kernels::MessageType get()
					{
						switch (message->Type)
						{
						case Models::Debug: return Kernels::MessageType::Debug;
						case Models::Info: return Kernels::MessageType::Info;
						case Models::Warning: return Kernels::MessageType::Warning;
						case Models::Error: return Kernels::MessageType::Error;
						default: throw gcnew System::NotSupportedException("Message type");
						}
					}
					void set(Kernels::MessageType value)
					{
						switch (value)
						{
						case Kernels::MessageType::Debug: message->Type = Models::MessageType::Debug; break;
						case Kernels::MessageType::Info: message->Type = Models::MessageType::Info; break;
						case Kernels::MessageType::Warning: message->Type = Models::MessageType::Warning; break;
						case Kernels::MessageType::Error: message->Type = Models::MessageType::Error; break;
						default: throw gcnew System::NotSupportedException("Message type");
						}
					}
				}

				property System::String^ Text
				{
					System::String^ get() { return NativeSupport::toManaged(message->Text); }
					void set(System::String^ value) { message->Text = NativeSupport::toNative(value); }
				}
			};
		}
	}
}

