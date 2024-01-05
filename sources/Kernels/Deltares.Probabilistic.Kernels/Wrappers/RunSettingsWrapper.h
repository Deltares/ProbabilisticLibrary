#pragma once
#include "../pch.h"
#include "../Model/RunSettings.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class RunSettingsWrapper
			{
			private:
				RunSettings* m_settings;

			public:
				RunSettingsWrapper() { m_settings = new RunSettings(); }
				~RunSettingsWrapper() { this->!RunSettingsWrapper(); }
				!RunSettingsWrapper() { delete m_settings; }

				property double MaxChunkSize
				{
					double get() { return m_settings->MaxChunkSize; }
					void set(double value) { m_settings->MaxChunkSize = value; }
				}

				property double MaxParallelProcesses
				{
					double get() { return m_settings->MaxParallelProcesses; }
					void set(double value) { m_settings->MaxParallelProcesses = value; }
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

				RunSettings* GetSettings()
				{
					return m_settings;
				}
			};
		}
	}
}


