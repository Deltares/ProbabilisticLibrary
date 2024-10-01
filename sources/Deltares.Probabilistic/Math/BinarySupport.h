#pragma once

#include <vector>

namespace Deltares::Mathematics
{
    class BinarySupport final
    {
        /// <summary>
        /// Gets a list where all combinations of two items are listed, in a given n-dimensional space
        /// Example: for n = 2 and items A and B, {A, A}, {A, B}, {B, A} and {B, B} is returned
        /// </summary>
        /// <param name="dimension"></param>
        /// <param name="item1"></param>
        /// <param name="item2"></param>
        /// <returns></returns>
    public:
        template<typename T>
        static std::vector<std::vector<T>> GetBinaryList(int dimension, T item1, T item2)
        {
            if (dimension == 0)
            {
                std::vector<std::vector<T>> valuesList;
                valuesList.push_back(std::vector<T>());

                return valuesList;
            }
            else
            {
                std::vector<std::vector<T>> valuesList;

                std::vector<std::vector<T>> subList = GetBinaryList(dimension - 1, item1, item2);

                for (auto newValue : std::vector<T> {item1, item2})
                {
                    for (auto list : subList)
                    {
                        std::vector<T> newList;
                        newList.push_back(newValue);
                        newList.insert(newList.end(), list.begin(), list.end());

                        valuesList.push_back(newList);
                    }
                }

                return valuesList;
            }
        }

        /// <summary>
        /// Returns the position of the rightmost zero bit of the number k
        /// </summary>
        /// <param name="k">a number</param>
        /// <returns>The rightmost zero bit</returns>
        static unsigned int RightmostZeroBit(unsigned int k);
    };
}

