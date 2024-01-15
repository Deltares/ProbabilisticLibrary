#pragma once

#include "../Math/Random.h"
#include "../Reliability/StochastListSettings.h"

class RandomSettings : public Deltares::Reliability::StochastListSettings
{
public:

    bool IsRepeatableRandom = true;

    /// <summary>
    /// Indicates whether random sequences is always the same for same <see cref="IHasStochast"/> names
    /// </summary>
    bool IsStochastRepeatableRandom = false;

    Deltares::Numeric::RandomValueGeneratorType RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::ModifiedKnuthSubtractive;

    /// <summary>
    /// Seed for randomizer
    /// </summary>
    int Seed = 0;

	/// <summary>
    /// Additional seed for randomizer
    /// </summary>
    int SeedB = 0;
};

