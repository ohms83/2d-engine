//
//  PlatformMacros.h
//  MiniRPG
//
//  Created by nuttachai on 6/6/20.
//

#ifndef PlatformMacros_h
#define PlatformMacros_h

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#endif /* PlatformMacros_h */
