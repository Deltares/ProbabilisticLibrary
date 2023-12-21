#pragma once
#include <vector>
#include "../distributions/distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        class listDistribs
        {
        public:
            listDistribs() : vectorSize(0) {};
            listDistribs(const int m) : vectorSize(m) {};
            ~listDistribs();
            void addDistrib(distrib* d);
            void updateDistrib(const size_t i, distrib* d);
            distrib* getDistrib(const size_t i);
            size_t size() const { return params.size(); }
            int getVectorSize() const { return vectorSize; }
        private:
            std::vector<distrib*> params;
            const int vectorSize;
        };
    }
}
