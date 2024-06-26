#pragma once
#include <memory>

#include "Message.h"
#include "ProxySettings.h"

namespace Deltares
{
	namespace Models
	{
        enum class designPointOptions
        {
            dpOutFALSE = 0,
            dpOutTRUE = 1,
            dpOutPrintAll = 3,
        };

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

			std::shared_ptr<ProxySettings> proxySettings = nullptr;

			bool isValid()
			{
				return MaxParallelProcesses >= 1;
			}

            bool IsProxyModel()
            {
                bool isProxyModel = false;
                if (proxySettings.get() != nullptr)
                {
                    isProxyModel = proxySettings->IsProxyModel;
                }
                return isProxyModel;
            }
		};
	}
}

