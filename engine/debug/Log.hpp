//
//  Log.hpp
//  MiniRPG
//
//  Created by nuttachai on 12/12/19.
//

#ifndef Log_hpp
#define Log_hpp

#include <memory>
#include <sstream>

#define LOG_ERROR(fmt, ...) debug::Log::error("[%s] " fmt, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) debug::Log::warning("[%s] " fmt, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) debug::Log::debug("%s" fmt, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#ifdef DEBUG
#define LOG_ASSERT(cond, fmt, ...) do { \
    if (!(cond)) { \
        debug::Log::error("[%s] Assertion FAILED! " fmt, __PRETTY_FUNCTION__, ##__VA_ARGS__); \
        assert(cond); \
    } \
} while(0);
#else   // DEBUG
#define LOG_ASSERT(cond, fmt, ...) do {} while(0);
#endif

namespace debug
{
    class Logger
    {
    public:
        virtual ~Logger() {}

        virtual void logDebug(const char* message) = 0;
        virtual void logWarning(const char* message) = 0;
        virtual void logError(const char* message) = 0;
    };
    
    /**
     *  Cross platform logging interface.
     */
    class Log
    {
    public:
        /**
         *  Initializing logging functions using the specified @c logger .
         *  Once initialzed, the ownership of @c logger will be taken away and the pointer will become undefine state.
         */
        static void init(Logger* logger, size_t bufferSize = 1048576 /* 1MB */);

        static void debug(const char* format, ...);
        static void warning(const char* format, ...);
        static void error(const char* format, ...);

        static void debug(const std::string& message);
        static void warning(const std::string& message);
        static void error(const std::string& message);

        static void debug(std::stringstream& message);
        static void warning(std::stringstream& message);
        static void error(std::stringstream& message);

    private:
        static std::unique_ptr<Logger> ms_singleton;
        static std::unique_ptr<char[]> ms_logBuffer;
        static size_t ms_bufferSize;
    };
}

#endif /* Log_hpp */
