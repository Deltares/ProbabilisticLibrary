#pragma once
namespace Deltares
{
    namespace Utils
    {
        namespace Wrappers
        {
            /// <summary>
            /// Repository of objects attached to a model sample or evaluation
            /// </summary>
            public ref class TagRepository
            {
            public:
                /// <summary>
                /// Gets a numeric identifier of an object
                /// </summary>
                int RegisterTag(System::Object^ object)
                {
                    objects[++counter] = object;
                    return counter;
                }

                /// <summary>
                /// Retrieves an object by an identifier
                /// </summary>
                System::Object^ RetrieveTag(int index)
                {
                    if (objects->ContainsKey(index))
                    {
                        return objects[index];
                    }
                    else
                    {
                        return nullptr;
                    }
                }
            private:
                int counter = 0;
                System::Collections::Generic::Dictionary<int, Object^>^ objects = gcnew System::Collections::Generic::Dictionary<int, Object^>();
            };
        }
    }
}

