// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once
#include "../../Deltares.Probabilistic/Model/ProxySettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            public ref class ProxySettings
            {

            private:
                Utils::Wrappers::SharedPointerProvider<Models::ProxySettings>* shared = new Utils::Wrappers::SharedPointerProvider(new Models::ProxySettings());

            public:
                ProxySettings() { }
                ~ProxySettings() { this->!ProxySettings(); }
                !ProxySettings() { delete shared; }

                property bool IsProxyModel
                {
                    bool get() { return shared->object->IsProxyModel; }
                    void set(bool value) { shared->object->IsProxyModel = value; }
                }

                property bool IsUpdatableProxyModel
                {
                    bool get() { return shared->object->IsUpdatableProxyModel; }
                    void set(bool value) { shared->object->IsUpdatableProxyModel = value; }
                }

                property bool ShouldUpdateFinalSteps
                {
                    bool get() { return shared->object->ShouldUpdateFinalSteps; }
                    void set(bool value) { shared->object->ShouldUpdateFinalSteps = value; }
                }

                property double ThresholdOffset
                {
                    double get() { return shared->object->ThresholdOffset; }
                    void set(double value) { shared->object->ThresholdOffset = value; }
                }

                std::shared_ptr<Models::ProxySettings> GetSettings()
                {
                    return shared->object;
                }
            };
        }
    }
}
