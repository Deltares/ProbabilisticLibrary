#pragma once
#include "../../Deltares.Probabilistic.Kernels/Model/RunSettings.h"
#include "../../Deltares.Probabilistic.Kernels/Model/Message.h"
#include "../Utils/SharedPointerProvider.h"
#include "MessageWrapper.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			using namespace Deltares::Models;

			public ref class RunSettingsWrapper
			{
			private:
				RunSettings* m_settings;
				SharedPointerProvider<RunSettings>* sharedPointer = new SharedPointerProvider<RunSettings>();

			public:
				RunSettingsWrapper() { m_settings = new RunSettings(); }
				~RunSettingsWrapper() { this->!RunSettingsWrapper(); }
				!RunSettingsWrapper() { delete sharedPointer; }

				property int MaxChunkSize
				{
					int get() { return m_settings->MaxChunkSize; }
					void set(int value) { m_settings->MaxChunkSize = value; }
				}

				property int MaxParallelProcesses
				{
					int get() { return m_settings->MaxParallelProcesses; }
					void set(int value) { m_settings->MaxParallelProcesses = value; }
				}

				property bool SaveConvergence
				{
					bool get() { return m_settings->SaveConvergence; }
					void set(bool value) { m_settings->SaveConvergence = value; }
				}

				property bool SaveEvaluations
				{
					bool get() { return m_settings->SaveEvaluations; }
					void set(bool value) { m_settings->SaveEvaluations = value; }
				}

				property bool SaveMessages
				{
					bool get() { return m_settings->SaveMessages; }
					void set(bool value) { m_settings->SaveMessages = value; }
				}

				property int MaxMessages
				{
					int get() { return m_settings->MaxMessages; }
					void set(int value) { m_settings->MaxMessages = value; }
				}

				property Kernels::MessageType LowestMessageType
				{
					Kernels::MessageType get()
					{
						switch (m_settings->LowestMessageType)
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
						case Kernels::MessageType::Debug: m_settings->LowestMessageType = Models::MessageType::Debug; break;
						case Kernels::MessageType::Info: m_settings->LowestMessageType = Models::MessageType::Info; break;
						case Kernels::MessageType::Warning: m_settings->LowestMessageType = Models::MessageType::Warning; break;
						case Kernels::MessageType::Error: m_settings->LowestMessageType = Models::MessageType::Error; break;
						default: throw gcnew System::NotSupportedException("Message type");
						}
					}
				}

				std::shared_ptr<RunSettings> GetSettings()
				{
					return sharedPointer->getSharedPointer(m_settings);
				}
			};
		}
	}
}


