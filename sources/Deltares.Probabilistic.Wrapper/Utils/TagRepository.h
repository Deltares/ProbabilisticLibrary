#pragma once
namespace Deltares
{
    namespace Utils
    {
        namespace Wrappers
        {
            public ref class TagRepository
            {
            public:
                int RegisterTag(System::Object^ object)
                {
                    objects[++counter] = object;
                    return counter;
                }

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

