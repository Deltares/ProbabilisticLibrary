// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include "../../Deltares.Probabilistic/Optimization/ClusterSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public enum class ClusterInitializationMethod {PlusPlus};

            public ref class ClusterSettings 
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Optimization::ClusterSettings>* shared = new Utils::Wrappers::SharedPointerProvider(new Optimization::ClusterSettings());
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

                std::shared_ptr<Optimization::ClusterSettings> GetSettings()
                {
                    return shared->object;
                }
            };
        }
    }
}




