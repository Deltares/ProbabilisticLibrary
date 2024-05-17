#pragma once

#include "../../Deltares.Probabilistic/Combine/DesignPointCombiner.h"
#include "../Model/Evaluation.h"
#include "../Model/StochastPointAlpha.h"
#include "../Model/Message.h"
#include "../Model/ProgressIndicator.h"
#include "../Model/RandomProvider.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "DesignPoint.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public enum class CombinerType
            {
                Hohenbichler,
                ImportanceSampling,
                DirectionalSampling
            };

            public enum class CombinationType
            {
                OneFails,
                AllFail
            };

            public ref class DesignPointCombiner
            {
            private:
                SharedPointerProvider<Reliability::DesignPointCombiner>* shared = new SharedPointerProvider(new Reliability::DesignPointCombiner());

                combineAndOr GetCombinationType(CombinationType combination)
                {
                    switch (combination)
                    {
                    case CombinationType::OneFails: return combineAndOr::combOr;
                    case CombinationType::AllFail: return combineAndOr::combAnd;
                    default: throw gcnew System::NotSupportedException("combination type");
                    }
                }

            public:
                DesignPointCombiner() {}
                ~DesignPointCombiner() { this->!DesignPointCombiner(); }
                !DesignPointCombiner() { delete shared; }

                property Wrappers::CombinerType CombinerType
                {
                    Wrappers::CombinerType get()
                    {
                        switch (shared->object->combinerType)
                        {
                        case Reliability::CombinerType::Hohenbichler: return Wrappers::CombinerType::Hohenbichler;
                        case Reliability::CombinerType::ImportanceSampling: return Wrappers::CombinerType::ImportanceSampling;
                        case Reliability::CombinerType::DirectionalSampling: return Wrappers::CombinerType::DirectionalSampling;
                        default: throw gcnew System::NotSupportedException("Combiner type");
                        }
                    }
                    void set(Wrappers::CombinerType value)
                    {
                        switch (value)
                        {
                        case Wrappers::CombinerType::Hohenbichler: shared->object->combinerType = Reliability::CombinerType::Hohenbichler; break;
                        case Wrappers::CombinerType::ImportanceSampling:shared->object->combinerType = Reliability::CombinerType::ImportanceSampling; break;
                        case Wrappers::CombinerType::DirectionalSampling: shared->object->combinerType = Reliability::CombinerType::DirectionalSampling; break;
                        default: throw gcnew System::NotSupportedException("Combiner type");
                        }
                    }
                }

                DesignPoint^ Combine(CombinationType combination, System::Collections::Generic::IList<Reliability::Wrappers::DesignPoint^>^ designPoints, Deltares::Statistics::Wrappers::CorrelationMatrix^ correlationMatrix, Wrappers::ProgressIndicator^ progressIndicator)
                {
                    RandomProvider::initialize();

                    std::vector<std::shared_ptr<Reliability::DesignPoint>> nativeDesignPoints;

                    System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts = gcnew System::Collections::Generic::List<Statistics::Wrappers::Stochast^>();

                    for (int i = 0; i < designPoints->Count; i++)
                    {
                        nativeDesignPoints.push_back(designPoints[i]->getDesignPoint());
                    }

                    const std::shared_ptr<Statistics::CorrelationMatrix> nativeCorrelationMatrix = correlationMatrix != nullptr ? correlationMatrix->GetCorrelationMatrix() : nullptr;
                    const std::shared_ptr<Models::ProgressIndicator> nativeProgressIndicator = progressIndicator != nullptr ? progressIndicator->GetProgressIndicator() : nullptr;
                    const combineAndOr combineAndOr = GetCombinationType(combination);

                    std::shared_ptr<Reliability::DesignPoint> nativeDesignPoint = shared->object->combineDesignPoints(combineAndOr, nativeDesignPoints, nativeCorrelationMatrix, nativeProgressIndicator);

                    Wrappers::DesignPoint^ designPoint = gcnew Wrappers::DesignPoint(nativeDesignPoint, designPoints);

                    return designPoint;
                }

                DesignPoint^ Combine(CombinationType combination, System::Collections::Generic::IList<Reliability::Wrappers::DesignPoint^>^ designPoints, Deltares::Statistics::Wrappers::CorrelationMatrix^ correlationMatrix)
                {
                    return Combine(combination, designPoints, correlationMatrix, nullptr);
                }

                DesignPoint^ Combine(CombinationType combination, System::Collections::Generic::IList<Reliability::Wrappers::DesignPoint^>^ designPoints)
                {
                    return Combine(combination, designPoints, nullptr, nullptr);
                }
            };
        }
    }
}
            

