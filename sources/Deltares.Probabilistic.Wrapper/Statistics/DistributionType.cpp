#include "DistributionType.h"
#include "../../Deltares.Probabilistic/Statistics/DistributionType.h"
#include "../../Deltares.Probabilistic/Statistics/DistributionPropertyType.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            Statistics::DistributionType DistributionTypeConverter::getNativeDistributionType(Wrappers::DistributionType distributionType)
            {
                switch (distributionType)
                {
                case Wrappers::DistributionType::Deterministic: return Statistics::DistributionType::Deterministic;
                case Wrappers::DistributionType::Normal: return Statistics::DistributionType::Normal;
                case Wrappers::DistributionType::LogNormal: return Statistics::DistributionType::LogNormal;
                case Wrappers::DistributionType::Uniform: return Statistics::DistributionType::Uniform;
                case Wrappers::DistributionType::Exponential: return Statistics::DistributionType::Exponential;
                case Wrappers::DistributionType::Triangular: return Statistics::DistributionType::Triangular;
                case Wrappers::DistributionType::Gumbel: return Statistics::DistributionType::Gumbel;
                case Wrappers::DistributionType::Weibull: return Statistics::DistributionType::Weibull;
                case Wrappers::DistributionType::ConditionalWeibull: return Statistics::DistributionType::ConditionalWeibull;
                case Wrappers::DistributionType::Frechet: return Statistics::DistributionType::Frechet;
                case Wrappers::DistributionType::GeneralizedExtremeValue: return Statistics::DistributionType::GeneralizedExtremeValue;
                case Wrappers::DistributionType::Rayleigh: return Statistics::DistributionType::Rayleigh;
                case Wrappers::DistributionType::RayleighN: return Statistics::DistributionType::RayleighN;
                case Wrappers::DistributionType::Table: return Statistics::DistributionType::Table;
                case Wrappers::DistributionType::CDFCurve: return Statistics::DistributionType::CDFCurve;
                case Wrappers::DistributionType::Discrete: return Statistics::DistributionType::Discrete;
                case Wrappers::DistributionType::Qualitative: return Statistics::DistributionType::Qualitative;
                default: throw gcnew System::NotSupportedException(distributionType.ToString());
                }
            }

            DistributionType DistributionTypeConverter::getManagedDistributionType(Statistics::DistributionType distributionType)
            {
                switch (distributionType)
                {
                case Statistics::DistributionType::Deterministic: return Wrappers::DistributionType::Deterministic;
                case Statistics::DistributionType::Normal: return Wrappers::DistributionType::Normal;
                case Statistics::DistributionType::LogNormal: return Wrappers::DistributionType::LogNormal;
                case Statistics::DistributionType::Uniform: return Wrappers::DistributionType::Uniform;
                case Statistics::DistributionType::Exponential: return Wrappers::DistributionType::Exponential;
                case Statistics::DistributionType::Triangular: return Wrappers::DistributionType::Triangular;
                case Statistics::DistributionType::Gumbel: return Wrappers::DistributionType::Gumbel;
                case Statistics::DistributionType::Weibull: return Wrappers::DistributionType::Weibull;
                case Statistics::DistributionType::ConditionalWeibull: return Wrappers::DistributionType::ConditionalWeibull;
                case Statistics::DistributionType::Frechet: return Wrappers::DistributionType::Frechet;
                case Statistics::DistributionType::GeneralizedExtremeValue: return Wrappers::DistributionType::GeneralizedExtremeValue;
                case Statistics::DistributionType::Rayleigh: return Wrappers::DistributionType::Rayleigh;
                case Statistics::DistributionType::RayleighN: return Wrappers::DistributionType::RayleighN;
                case Statistics::DistributionType::Table: return Wrappers::DistributionType::Table;
                case Statistics::DistributionType::CDFCurve: return Wrappers::DistributionType::CDFCurve;
                case Statistics::DistributionType::Discrete: return Wrappers::DistributionType::Discrete;
                case Statistics::DistributionType::Qualitative: return Wrappers::DistributionType::Qualitative;
                default: throw gcnew System::NotSupportedException("distribution type");
                }
            }

            Statistics::DistributionPropertyType DistributionTypeConverter::getNativeDistributionPropertyType(Wrappers::DistributionPropertyType distributionPropertyType)
            {
                switch (distributionPropertyType)
                {
                case Wrappers::DistributionPropertyType::Location: return Statistics::DistributionPropertyType::Location;
                case Wrappers::DistributionPropertyType::Scale: return Statistics::DistributionPropertyType::Scale;
                case Wrappers::DistributionPropertyType::Minimum: return Statistics::DistributionPropertyType::Minimum;
                case Wrappers::DistributionPropertyType::Maximum: return Statistics::DistributionPropertyType::Maximum;
                case Wrappers::DistributionPropertyType::Shift: return Statistics::DistributionPropertyType::Shift;
                case Wrappers::DistributionPropertyType::ShiftB: return Statistics::DistributionPropertyType::ShiftB;
                case Wrappers::DistributionPropertyType::Shape: return Statistics::DistributionPropertyType::Shape;
                case Wrappers::DistributionPropertyType::ShapeB: return Statistics::DistributionPropertyType::ShapeB;
                case Wrappers::DistributionPropertyType::Observations: return Statistics::DistributionPropertyType::Observations;
                default: throw gcnew System::NotSupportedException(distributionPropertyType.ToString());
                }
            }
        }
    }
}


