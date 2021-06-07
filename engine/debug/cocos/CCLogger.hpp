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
        void logDebug(const char* message) override
        {
            CCLOG( "%s", message );
        }
        void logWarning(const char* message) override
        {
            CCLOGWARN( "%s", message );
        }
        void logError(const char* message) override
        {
            CCLOGERROR( "%s", message );
        }
    };
}

#endif /* CCLogger_hpp */
