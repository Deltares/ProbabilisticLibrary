#pragma once
#include "../../Deltares.Probabilistic/Model/RandomSettings.h"
#include "../Utils/SharedPointerProvider.h"
#include "RandomProvider.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Models;
			using namespace Deltares::Utils::Wrappers;

            public enum class RandomGeneratorType { MersenneTwister, GeorgeMarsaglia, ModifiedKnuthSubtractive };

			public ref class RandomSettings
			{
			private:
				SharedPointerProvider<Deltares::Models::RandomSettings>* shared = new SharedPointerProvider(new Deltares::Models::RandomSettings());

				bool hasLimitedRandomValues = false;
				bool isStochastRepeatableRandom = false;

			public:
				RandomSettings()
				{
					RandomProvider::initialize();
				}
				~RandomSettings() { this->!RandomSettings(); }
				!RandomSettings() { delete shared; }

				property int Seed
				{
					int get() { return shared->object->Seed; }
					void set(int value) { shared->object->Seed = value; }
				}

				property int SeedB
				{
					int get() { return shared->object->SeedB; }
					void set(int value) { shared->object->SeedB = value; }
				}

				property bool IsRepeatableRandom
				{
					bool get() { return shared->object->IsRepeatableRandom; }
					void set(bool value) { shared->object->IsRepeatableRandom = value; }
				}

				/// <summary>
				/// Indicates whether the randomizer should draw dummy random values for unvarying parameters,
				/// so that changing distributions from varying to unvarying wil not affect the random values of varying parameters
				/// </summary>
				property bool SkipUnvaryingParameters
				{
					bool get() { return shared->object->SkipUnvaryingParameters; }
					void set(bool value) { shared->object->SkipUnvaryingParameters = value; }
				}

                property Wrappers::RandomGeneratorType RandomGeneratorType
                {
                    Wrappers::RandomGeneratorType get()
                    {
                        switch (shared->object->RandomGeneratorType)
                        {
                        case Numeric::RandomValueGeneratorType::MersenneTwister: return Models::Wrappers::RandomGeneratorType::MersenneTwister;
                        case Numeric::RandomValueGeneratorType::GeorgeMarsaglia: return Models::Wrappers::RandomGeneratorType::GeorgeMarsaglia;
                        case Numeric::RandomValueGeneratorType::ModifiedKnuthSubtractive: return Models::Wrappers::RandomGeneratorType::ModifiedKnuthSubtractive;
                        default: throw gcnew System::NotSupportedException("Random generator type");
                        }
                    }
                    void set(Wrappers::RandomGeneratorType value)
                    {
                        switch (value)
                        {
                        case  Wrappers::RandomGeneratorType::MersenneTwister: shared->object->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::MersenneTwister; break;
                        case  Wrappers::RandomGeneratorType::GeorgeMarsaglia: shared->object->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::GeorgeMarsaglia; break;
                        case  Wrappers::RandomGeneratorType::ModifiedKnuthSubtractive: shared->object->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::ModifiedKnuthSubtractive; break;
                        default: throw gcnew System::NotSupportedException("Design point method");
                        }
                    }
                }

				// TODO: PROBL-42 remove this property when RandomValueGenerator is not used any more

				/// <summary>
				/// Indicates whether random values a limited to a certain range
				/// </summary>
				property bool HasLimitedRandomValues
				{
					bool get() { return hasLimitedRandomValues; }
					void set(bool value) { hasLimitedRandomValues = value; }
				}

                // TODO: PROBL-42 remove this property when RandomValueGenerator is not used any more
                /// <summary>
				/// Indicates whether random sequences is always the same for same <see cref="IHasStochast"/> names
				/// </summary>
				property bool IsStochastRepeatableRandom
				{
					bool get() { return isStochastRepeatableRandom; }
					void set(bool value) { isStochastRepeatableRandom = value; }
				}


				std::shared_ptr<Models::RandomSettings> GetSettings()
				{
					return shared->object;
				}
			};

			public interface class IHasRandomSettings
			{
				property Wrappers::RandomSettings^ RandomSettings;
			};
		}
	}
}


