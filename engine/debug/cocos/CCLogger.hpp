//
//  CCLogger.hpp
//
//  Created by nuttachai on 12/02/21.
//

#ifndef CCLogger_hpp
#define CCLogger_hpp

#include "engine/debug/Log.hpp"
#include "cocos2d.h"

namespace debug
{
    class CCLogger : public Logger
    {
    public:
        void logDebug(const char* message) const override;
        void logInfo(const char* message) const override;
        void logWarning(const char* message) const override;
        void logError(const char* message) const override;
    };
}

#endif /* CCLogger_hpp */
