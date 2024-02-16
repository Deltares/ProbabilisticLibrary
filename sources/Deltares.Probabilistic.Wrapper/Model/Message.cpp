#include "Message.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			Wrappers::MessageType Message::getMessageType(Deltares::Models::MessageType messageType)
			{
				switch (messageType)
				{
				case Models::Debug: return Wrappers::MessageType::Debug;
				case Models::Info: return Wrappers::MessageType::Info;
				case Models::Warning: return Wrappers::MessageType::Warning;
				case Models::Error: return Wrappers::MessageType::Error;
				default: throw gcnew System::NotSupportedException("Message type");
				}
			}
		}
	}
}

