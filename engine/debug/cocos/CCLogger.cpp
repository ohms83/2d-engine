#include "CCLogger.hpp"
#include "../DebugConst.hpp"

namespace debug
{
    void CCLogger::logDebug(const char* message) const
    {
        cocos2d::log( "%s", message );
    }
    void CCLogger::logInfo(const char* message) const
    {
        cocos2d::log( "%s", message );
    }
    void CCLogger::logWarning(const char* message) const
    {
        cocos2d::log( "%sWARNING: %s%s", DEBUG_WARNING_TAG, message, TAG_CLOSE );
    }
    void CCLogger::logError(const char* message) const
    {
        cocos2d::log( "%sERROR: %s%s", DEBUG_ERROR_TAG, message, TAG_CLOSE );
    }
}