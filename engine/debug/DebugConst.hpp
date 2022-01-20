//
//  DebugConst.hpp
//  MiniRPG
//
//  Created by nuttachai on 12/12/19.
//

#ifndef DebugConst_hpp
#define DebugConst_hpp

#ifdef DEBUG
    #ifdef LOG_COLOR_SCHEME
        #define DEBUG_WARNING_TAG "\033[1;33m"
        #define DEBUG_ERROR_TAG "\033[1;31m"
        #define TAG_CLOSE "\033[0m"
    #elif defined(LOG_EMOJI)
        #define DEBUG_WARNING_TAG "⚠️"
        #define DEBUG_ERROR_TAG "🛑"
        #define TAG_CLOSE ""
    #else
        #define DEBUG_WARNING_TAG ""
        #define DEBUG_ERROR_TAG ""
        #define TAG_CLOSE ""
    #endif
#else
    #if defined(LOG_EMOJI)
        #define DEBUG_WARNING_TAG "⚠️"
        #define DEBUG_ERROR_TAG "🛑"
        #define TAG_CLOSE ""
    #else
        #define DEBUG_WARNING_TAG ""
        #define DEBUG_ERROR_TAG ""
        #define TAG_CLOSE ""
    #endif
#endif

namespace debug {
    
}

#endif /* DebugConst_hpp */
