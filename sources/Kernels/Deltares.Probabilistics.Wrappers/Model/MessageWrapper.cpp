#include "MessageWrapper.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			Kernels::MessageType MessageWrapper::getMessageType(Deltares::Models::MessageType messageType)
			{
				switch (messageType)
				{
				case Models::Debug: return Kernels::MessageType::Debug;
				case Models::Info: return Kernels::MessageType::Info;
				case Models::Warning: return Kernels::MessageType::Warning;
				case Models::Error: return Kernels::MessageType::Error;
				default: throw gcnew System::NotSupportedException("Message type");
				}
			}
		}
	}
}

