//
//  Debug.h
//  MiniRPG
//
//  Created by nuttachai on 12/12/19.
//

#ifndef _Debug_Macros_h_
#define _Debug_Macros_h_

#include <memory>
#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <iostream>

#include "../platform/NativeUI.h"
#include "../platform/PlatformMacros.h"

#define MAX_LOG_BUFFER 1024*1024 // Allocating 1kB for error log

#ifdef DEBUG // Debug build
#define ALERT(cond) \
{ \
    if(!cond) { \
        std::string errorMsg = std::move(std::string(__PRETTY_FUNCTION__).append("\n").append(std::string(#cond))); \
        platform::showNativePopup("Error", errorMsg); \
        CCLOG("Error: %s", errorMsg.c_str()); \
    } \
}

#define ALERT_MSG(cond, fmt, ...) \
{ \
    if(!cond) { \
        std::unique_ptr<char> buffer(new char[MAX_LOG_BUFFER]); \
        snprintf (buffer.get(), MAX_LOG_BUFFER, fmt, ##__VA_ARGS__); \
        std::string errorMsg = std::move(std::string(__PRETTY_FUNCTION__).append("\n").append(buffer.get())); \
        platform::showNativePopup("Error", errorMsg); \
        CCLOG("Error: %s", errorMsg.c_str()); \
    } \
}

#define NOTICE_WARNING(fmt, ...) \
{ \
    std::unique_ptr<char> buffer(new char[MAX_LOG_BUFFER]); \
    snprintf (buffer.get(), MAX_LOG_BUFFER, fmt, ##__VA_ARGS__); \
    std::string warningMsg = std::move(std::string(__PRETTY_FUNCTION__).append("\n").append(buffer.get())); \
    platform::showNativePopup("Warning", warningMsg); \
    CCLOG("Warning: %s", warningMsg.c_str()); \
}

#elif defined(UNIT_TEST) // Unit Test
#define ALERT(cond) assert(cond) // In UNIT_TEST environment, this macro is just an alias to 'assert'.

#define ALERT_MSG(cond, fmt, ...) \
{ \
    if(!cond) { \
        std::unique_ptr<char> buffer(new char[MAX_LOG_BUFFER]); \
        snprintf (buffer.get(), MAX_LOG_BUFFER, fmt, ##__VA_ARGS__); \
        std::string errorMsg = std::move(std::string(__PRETTY_FUNCTION__).append("\n").append(buffer.get())); \
        std::cout << "ASSERT: " << errorMsg << std::endl; \
        assert(cond); \
    } \
}

#define NOTICE_WARNING(fmt, ...) \
{ \
    std::unique_ptr<char> buffer(new char[MAX_LOG_BUFFER]); \
    snprintf (buffer.get(), MAX_LOG_BUFFER, fmt, ##__VA_ARGS__); \
    std::string warningMsg = std::move(std::string(__PRETTY_FUNCTION__).append("\n").append(buffer.get())); \
    std::cout << "WARNING: " << warningMsg << std::endl; \
}

#else // Release build

#define ALERT(cond) // NOOP
#define ALERT_MSG(cond, fmt, ...) // NOOP
#define NOTICE_WARNING(fmt, ...) // NOOP

#endif
#endif /* _Debug_Macros_h_ */
