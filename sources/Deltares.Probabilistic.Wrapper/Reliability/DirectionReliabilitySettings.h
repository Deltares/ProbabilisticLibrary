#pragma once

#include "DesignPoint.h"
#include "../../Deltares.Probabilistic/Reliability/DirectionReliabilitySettings.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public enum class VaryingType
            {
                Monotone,
                Varying
            };


            public ref class DirectionReliabilitySettings
            {
            private:
                SharedPointerProvider<Reliability::DirectionReliabilitySettings>* shared = new SharedPointerProvider(new Reliability::DirectionReliabilitySettings());
            public:
                DirectionReliabilitySettings() {}
                ~DirectionReliabilitySettings() { this->!DirectionReliabilitySettings(); }
                !DirectionReliabilitySettings() { delete shared; }

                property double Dsdu
                {
                    double get() { return shared->object->Dsdu; }
                    void set(double value) { shared->object->Dsdu = value; }
                }

                property double EpsilonUStepSize
                {
                    double get() { return shared->object->EpsilonUStepSize; }
                    void set(double value) { shared->object->EpsilonUStepSize = value; }
                }

                property double EpsilonZStepSize
                {
                    double get() { return shared->object->EpsilonZStepSize; }
                    void set(double value) { shared->object->EpsilonZStepSize = value; }
                }

                property int MaximumIterations
                {
                    int get() { return shared->object->MaximumIterations; }
                    void set(int value) { shared->object->MaximumIterations = value; }
                }

                property double MaximumLengthU
                {
                    double get() { return shared->object->MaximumLengthU; }
                    void set(double value) { shared->object->MaximumLengthU = value; }
                }

                property Wrappers::VaryingType ModelVaryingType
                {
                    Wrappers::VaryingType get()
                    {
                        switch (shared->object->modelVaryingType)
                        {
                        case Reliability::Monotone: return Wrappers::VaryingType::Monotone;
                        case Reliability::Varying: return Wrappers::VaryingType::Varying;
                        default: throw gcnew System::NotSupportedException("Model varying type");
                        }
                    }
                    void set(Wrappers::VaryingType value)
                    {
                        switch (value)
                        {
                        case Wrappers::VaryingType::Monotone: shared->object->modelVaryingType = Reliability::Monotone; break;
                        case Wrappers::VaryingType::Varying: shared->object->modelVaryingType = Reliability::Varying; break;
                        default: throw gcnew System::NotSupportedException("Model varying type");
                        }
                    }
                }

                void SetStartPoint(DesignPoint^ designPoint)
                {
                    shared->object->StochastSet = std::make_shared<Reliability::StochastSettingsSet>(designPoint->getDesignPoint());
                }

                std::shared_ptr<Reliability::DirectionReliabilitySettings> GetSettings()
                {
                    return shared->object;
                }
            };
        }
    }
}




