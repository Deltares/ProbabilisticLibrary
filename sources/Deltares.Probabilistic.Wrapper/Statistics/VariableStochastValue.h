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

#include "../../Deltares.Probabilistic/Statistics/VariableStochastValue.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public ref class VariableStochastValue : System::IComparable<VariableStochastValue^>
            {
            private:
                SharedPointerProvider<Statistics::VariableStochastValue>* shared = new SharedPointerProvider(new Statistics::VariableStochastValue());

            public:
                VariableStochastValue()    {}
                ~VariableStochastValue() { this->!VariableStochastValue(); }
                !VariableStochastValue() { delete shared; }

                property double X
                {
                    double get() { return shared->object->X; }
                    void set(double value) { shared->object->X = value; }
                }

                property double Mean
                {
                    double get() { return shared->object->mean; }
                    void set(double value) { shared->object->mean = value; }
                }

                property double Deviation
                {
                    double get() { return shared->object->deviation; }
                    void set(double value) { shared->object->deviation = value; }
                }

                property double Location
                {
                    double get() { return shared->object->Stochast->Location; }
                    void set(double value) { shared->object->Stochast->Location = value; }
                }

                property double Scale
                {
                    double get() { return shared->object->Stochast->Scale; }
                    void set(double value) { shared->object->Stochast->Scale = value; }
                }

                property double Shift
                {
                    double get() { return shared->object->Stochast->Shift; }
                    void set(double value) { shared->object->Stochast->Shift = value; }
                }

                property double ShiftB
                {
                    double get() { return shared->object->Stochast->ShiftB; }
                    void set(double value) { shared->object->Stochast->ShiftB = value; }
                }

                property double Shape
                {
                    double get() { return shared->object->Stochast->Shape; }
                    void set(double value) { shared->object->Stochast->Shape = value; }
                }

                property double ShapeB
                {
                    double get() { return shared->object->Stochast->ShapeB; }
                    void set(double value) { shared->object->Stochast->ShapeB = value; }
                }

                property double Minimum
                {
                    double get() { return shared->object->Stochast->Minimum; }
                    void set(double value) { shared->object->Stochast->Minimum = value; }
                }

                property double Maximum
                {
                    double get() { return shared->object->Stochast->Maximum; }
                    void set(double value) { shared->object->Stochast->Maximum = value; }
                }

                property int Observations
                {
                    int get() { return shared->object->Stochast->Observations; }
                    void set(int value) { shared->object->Stochast->Observations = value; }
                }

                std::shared_ptr<Statistics::VariableStochastValue> GetValue()
                {
                    return shared->object;
                }

                virtual int CompareTo(VariableStochastValue^ other)
                {
                    return this->X.CompareTo(other->X);
                }
            };
        }
    }
}








