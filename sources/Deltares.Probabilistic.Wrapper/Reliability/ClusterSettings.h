#pragma once

#include "../../Deltares.Probabilistic/Optimization/ClusterSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public enum class ClusterInitializationMethod {PlusPlus};

			public ref class ClusterSettings 
			{
			private:
				SharedPointerProvider<Optimization::ClusterSettings>* shared = new SharedPointerProvider(new Optimization::ClusterSettings());
			public:
				ClusterSettings() { }
				~ClusterSettings() { this->!ClusterSettings(); }
				!ClusterSettings() { delete shared; }

				property Wrappers::ClusterInitializationMethod ClusterInitializationMethod
				{
					Wrappers::ClusterInitializationMethod get()
					{
						switch (shared->object->clusterInitializationMethod)
						{
						case Optimization::ClusterInitializationMethod::PlusPlus: return Wrappers::ClusterInitializationMethod::PlusPlus;
						default: throw gcnew System::NotSupportedException("Cluster initialization method");
						}
					}
					void set(Wrappers::ClusterInitializationMethod value)
					{
						switch (value)
						{
						case Wrappers::ClusterInitializationMethod::PlusPlus: shared->object->clusterInitializationMethod = Optimization::ClusterInitializationMethod::PlusPlus; break;
						default: throw gcnew System::NotSupportedException("Cluster initialization method");
						}
					}
				}

				property int MaxClusters
				{
					int get() { return shared->object->MaxClusters; }
					void set(int value) { shared->object->MaxClusters = value; }
				}

				property bool OptimizeNumberClusters
				{
					bool get() { return shared->object->OptimizeNumberOfClusters; }
					void set(bool value) { shared->object->OptimizeNumberOfClusters = value; }
				}

				bool IsValid()
				{
					return shared->object->isValid();
				}

				std::shared_ptr<Optimization::ClusterSettings> GetSettings()
				{
					return shared->object;
				}
			};
		}
	}
}




