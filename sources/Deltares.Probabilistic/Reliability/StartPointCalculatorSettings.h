#pragma once
#include <vector>
#include "StochastSettingsSet.h"

namespace Deltares
{
    namespace Reliability
    {
        enum StartMethodType { None, One, RaySearch, SensitivitySearch, SphereSearch, GivenVector };

        /**
         * \brief Settings for the start point calculator
         */
        class StartPointCalculatorSettings
        {
        public:
            /**
             * \brief Type of start point calculation
             */
            StartMethodType StartMethod = StartMethodType::None;
            double MaximumLengthStartPoint = 6;
            double GradientStepSize = 4;
            double RadiusSphereSearch = 10;
            double dsdu = 1;
            bool allQuadrants = false;

            std::shared_ptr<StartPointCalculatorSettings> clone()
            {
                std::shared_ptr<StartPointCalculatorSettings> copy = std::make_shared<StartPointCalculatorSettings>();

                copy->GradientStepSize = this->GradientStepSize;
                copy->MaximumLengthStartPoint = this->MaximumLengthStartPoint;
                copy->RadiusSphereSearch = this->RadiusSphereSearch;
                copy->StartMethod = this->StartMethod;
                copy->allQuadrants = this->allQuadrants;

                copy->StochastSet = this->StochastSet;

                return copy;
            }

            bool isValid()
            {
                return StartMethod == StartMethodType::None ||
                    (StartMethod == StartMethodType::RaySearch && MaximumLengthStartPoint >= 1) ||
                    (StartMethod == StartMethodType::SensitivitySearch && MaximumLengthStartPoint >= 1) ||
                    (StartMethod == StartMethodType::SphereSearch && RadiusSphereSearch >= 0.1);
            }

            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();
            std::vector<double> startVector;

            static std::string getStartPointMethodString(StartMethodType method);
            static StartMethodType getStartPointMethod(std::string method);
        };
    }
}
