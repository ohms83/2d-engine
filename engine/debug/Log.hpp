//
//  Log.hpp
//  MiniRPG
//
//  Created by nuttachai on 12/12/19.
//

#ifndef Log_hpp
#define Log_hpp

#include <memory>
#include <vector>
#include <sstream>

#define LOG_ERROR(fmt, ...) debug::Log::error("[%s] " fmt, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) debug::Log::warning("[%s] " fmt, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) debug::Log::info(fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) debug::Log::debug(fmt, ##__VA_ARGS__)

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

        /**
         *  Debug log will only being printed when log level is @c VERBOSE and @c DEBUG flag is enabled by build variant.
         *  It's safe to use this function to print out sensitive information (transaction ID, player's info, etc.) since
         *  it won't be printed out in the release builds
         */
        virtual void logDebug(const char* message) const = 0;
        /**
         *  Info log can be considered a debug log that can be used for debugging release builds. For example, if you are
         *  developing error reporting module that printing out any useful information to help you catching bugs on player's end,
         *  please consider printing out those logs using this function.
         *  Please note that, the logs might be exposed by player and recording sensitive information should be handled carefully.
         */
        virtual void logInfo(const char* message) const = 0;
        /**
         *  Use this function to log any kind of errors that can easily be recovered or are not critical.
         */
        virtual void logWarning(const char* message) const = 0;
        /**
         *  Logging any critical errors that requires immediate attention.
         */
        virtual void logError(const char* message) const = 0;
    };
    
    /**
     *  Cross platform logging interface.
     */
    class Log
    {
    public:
        //! Define log level
        enum Level
        {
            //! All logs will be printed
            VERBOSE,
            //! Only info, warning and error logs will be printed
            INFO,
            //! Only warning and error logs will be printed
            WARNING,
            //! Only error logs will be printed
            ERROR,
        };
        /**
         *  Initializing logging functions using the specified @c logger .
         *  Once initialzed, the ownership of @c logger will be taken away and the pointer will become undefine state.
         */
        static void init(Logger* logger, size_t bufferSize = 1048576 /* 1MB */);
        // Register logger. The ownership of @c logger will be taken away and the pointer will become undefine state.
        static void registerLogger(Logger* logger);

        static void debug(const char* format, ...);
        static void info(const char* format, ...);
        static void warning(const char* format, ...);
        static void error(const char* format, ...);

        static void debug(const std::string& message);
        static void info(const std::string& message);
        static void warning(const std::string& message);
        static void error(const std::string& message);

        static void debug(std::stringstream& message);
        static void info(std::stringstream& message);
        static void warning(std::stringstream& message);
        static void error(std::stringstream& message);

        // Filter out what kind of log will be printed.
        static void setLogLevel(Level level);
        static Level getLogLevel();

    private:
        typedef std::unique_ptr<Logger> LoggerPtr; 

        static std::vector<LoggerPtr> ms_loggers;
        static std::unique_ptr<char[]> ms_logBuffer;
        static size_t ms_bufferSize;
        static Level ms_logLevel;
    };
}

#endif /* Log_hpp */
