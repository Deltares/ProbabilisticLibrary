#pragma once
namespace Deltares
{
    namespace Numeric
    {
        class WeightedValue
        {
        public:
            WeightedValue(double value, double weight)
            {
                this->value = value;
                this->weight = weight;
            }

            double value;
            double weight;
        };

    }
}

