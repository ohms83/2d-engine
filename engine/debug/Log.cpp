//
//  DebugConst.cpp
//  MiniRPG
//
//  Created by nuttachai on 12/12/19.
//

#include "Log.hpp"
#include <cstdarg>

using namespace std;
using namespace debug;

#define CHECK_IF_INIT() \
if(!ms_logBuffer || ms_loggers.size() == 0) { \
    return; \
}

#define PRINT_TO_BUFFER() \
va_list args; \
va_start(args, format); \
vsnprintf(ms_logBuffer.get(), ms_bufferSize, format, args); \
va_end(args);

#define PRINT_LOG(_logLevel, _printFunction, _msg) \
if((int)_logLevel >= (int)ms_logLevel) { \
    for(const auto& logger : ms_loggers) { \
        if(logger) { \
            logger->_printFunction(_msg); \
        } \
    } \
}

size_t Log::ms_bufferSize = 0;
vector<Log::LoggerPtr> Log::ms_loggers;
unique_ptr<char[]> Log::ms_logBuffer;
Log::Level Log::ms_logLevel = Log::Level::VERBOSE;

void Log::init(Logger* logger, size_t bufferSize)
{
    ms_loggers.emplace_back(logger);
    ms_logBuffer.reset(new char[bufferSize]);
    ms_bufferSize = bufferSize;
}

void Log::registerLogger(Logger* logger)
{
    ms_loggers.emplace_back(logger);
}

void Log::debug(const char* format, ...)
{
#ifdef DEBUG
    CHECK_IF_INIT();
    PRINT_TO_BUFFER();
    PRINT_LOG(Level::VERBOSE, logDebug, ms_logBuffer.get());
#endif
}

void Log::info(const char* format, ...)
{
    CHECK_IF_INIT();
    PRINT_TO_BUFFER();
    PRINT_LOG(Level::INFO, logInfo, ms_logBuffer.get());
}

void Log::warning(const char* format, ...)
{
    CHECK_IF_INIT();
    PRINT_TO_BUFFER();
    PRINT_LOG(Level::WARNING, logWarning, ms_logBuffer.get());
}

void Log::error(const char* format, ...)
{
    CHECK_IF_INIT();
    PRINT_TO_BUFFER();
    PRINT_LOG(Level::ERROR, logError, ms_logBuffer.get());
}

void Log::debug(const string& message)
{
#ifdef DEBUG
    CHECK_IF_INIT();
    PRINT_LOG(Level::VERBOSE, logDebug, message.c_str());
#endif
}
void Log::info(const string& message)
{
    CHECK_IF_INIT();
    PRINT_LOG(Level::INFO, logInfo, message.c_str());
}
void Log::warning(const string& message)
{
    CHECK_IF_INIT();
    PRINT_LOG(Level::WARNING, logWarning, message.c_str());
}
void Log::error(const string& message)
{
    CHECK_IF_INIT();
    PRINT_LOG(Level::ERROR, logError, message.c_str());
}

void Log::debug(stringstream& message)
{
#ifdef DEBUG
    CHECK_IF_INIT();
    PRINT_LOG(Level::VERBOSE, logDebug, message.str().c_str());
#endif
}
void Log::info(stringstream& message)
{
    CHECK_IF_INIT();
    PRINT_LOG(Level::INFO, logInfo, message.str().c_str());
}
void Log::warning(stringstream& message)
{
    CHECK_IF_INIT();
    PRINT_LOG(Level::WARNING, logWarning, message.str().c_str());
}
void Log::error(stringstream& message)
{
    CHECK_IF_INIT();
    PRINT_LOG(Level::ERROR, logError, message.str().c_str());
}
