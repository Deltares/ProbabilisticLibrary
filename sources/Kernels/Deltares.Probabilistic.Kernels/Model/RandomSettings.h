#pragma once
class RandomSettings
{
public:

    bool IsRepeatableRandom = true;

    /// <summary>
    /// Indicates whether random sequences is always the same for same <see cref="IHasStochast"/> names
    /// </summary>
    bool IsStochastRepeatableRandom = false;

    /// <summary>
    /// Seed for randomizer
    /// </summary>
    int Seed = 0;
};

