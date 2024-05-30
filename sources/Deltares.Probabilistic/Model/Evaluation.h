#pragma once

#include <cmath>
#include <cstdint>

#include "../Utils/TagSupport.h"

namespace Deltares
{
	namespace Models
	{
		class Evaluation
		{
		public:
            Evaluation() = default;
            ~Evaluation()
            {
                if (isOwnerOfTag)
                {
                    Utils::TagSupport::releaseTag(Tag);
                }
            }

			double Z = nan("");
			double Weight = 1;
			int Iteration = -1;

            bool isOwnerOfTag = true;
			intptr_t Tag = 0;
		};
	}
}

