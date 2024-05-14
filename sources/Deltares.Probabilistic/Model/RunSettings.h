#pragma once
#include <memory>

#include "Message.h"
#include "ProxySettings.h"

namespace Deltares
{
	namespace Models
	{
		class RunSettings
		{
		public:
			int MaxParallelProcesses = 1;
			int MaxChunkSize = 16;
			bool SaveEvaluations = false;
			bool SaveConvergence = false;
			bool SaveMessages = false;
			int MaxMessages = 1000;
			MessageType LowestMessageType = MessageType::Warning;

			std::shared_ptr<ProxySettings> proxySettings = std::make_shared<Models::ProxySettings>();

			bool isValid()
			{
				return MaxParallelProcesses >= 1;
			}
		};
	}
}

