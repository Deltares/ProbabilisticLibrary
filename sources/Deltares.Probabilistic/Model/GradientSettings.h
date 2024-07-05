#pragma once

namespace Deltares
{
    namespace Models
    {
        enum GradientType
        {
            OneDirection,
            TwoDirections
        };

        class GradientSettings
        {
        public:
            GradientSettings() {}

            GradientType gradientType = OneDirection;
            double StepSize = 0.3;
            bool OnlyInitializationAllowed = false;

            bool isValid()
            {
                return StepSize >= 0.01;
            }
        };
    }
}


