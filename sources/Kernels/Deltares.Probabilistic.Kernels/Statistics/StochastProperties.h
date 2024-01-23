#pragma once

#ifndef StochastProperties_H
#define StochastProperties_H
#endif

namespace Deltares
{
    namespace Statistics
    {
        class StochastProperties
        {

        public:

            double Location;
            double Scale;
            double Minimum;
            double Maximum;
            double Shift;
            double ShiftB;
            double Shape;
            double ShapeB;
            int Observations;
        };
    }
}
