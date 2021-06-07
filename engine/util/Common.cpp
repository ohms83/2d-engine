#include "Common.hpp"

namespace util
{
    namespace common
    {
        bool runTest(std::vector<std::function<bool(void)>> testCases)
        {
            for (auto testCase : testCases)
            {
                if (!testCase()) {
                    return false;
                }
            }
            return true;
        }
    }
}
