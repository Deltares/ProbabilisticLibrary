#pragma once
namespace Deltares
{
    namespace Statistics
    {
        /**
         * \brief Base class of a StochastPoint and DesignPoint.
         * \remark This class was introduced to prevent circular references (via FragilityValue)
         */
        class BaseStochastPoint
        {
        public:
            double Beta;
        };
    }
}

