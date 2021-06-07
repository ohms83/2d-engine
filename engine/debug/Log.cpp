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
if(!ms_logBuffer || !ms_singleton) { \
    return; \
}

#define PRINT_TO_BUFFER() \
va_list args; \
va_start(args, format); \
vsnprintf(ms_logBuffer.get(), ms_bufferSize, format, args); \
va_end(args);

size_t Log::ms_bufferSize = 0;
unique_ptr<Logger> Log::ms_singleton;
unique_ptr<char[]> Log::ms_logBuffer;

void Log::init(Logger* logger, size_t bufferSize)
{
    ms_singleton.reset(logger);
    ms_logBuffer.reset(new char[bufferSize]);
    ms_bufferSize = bufferSize;
}

void Log::debug(const char* format, ...)
{
    CHECK_IF_INIT();
    PRINT_TO_BUFFER();
    ms_singleton->logDebug(ms_logBuffer.get());
}

void Log::warning(const char* format, ...)
{
    CHECK_IF_INIT();
    PRINT_TO_BUFFER();
    ms_singleton->logWarning(ms_logBuffer.get());
}

void Log::error(const char* format, ...)
{
    CHECK_IF_INIT();
    PRINT_TO_BUFFER();
    ms_singleton->logError(ms_logBuffer.get());
}

void Log::debug(const string& message)
{
    CHECK_IF_INIT();
    ms_singleton->logDebug(message.c_str());
}
void Log::warning(const string& message)
{
    CHECK_IF_INIT();
    ms_singleton->logWarning(message.c_str());
}
void Log::error(const string& message)
{
    CHECK_IF_INIT();
    ms_singleton->logError(message.c_str());
}

void Log::debug(stringstream& message)
{
    CHECK_IF_INIT();
    ms_singleton->logDebug(message.str().c_str());
}

void Log::warning(stringstream& message)
{
    CHECK_IF_INIT();
    ms_singleton->logWarning(message.str().c_str());
}

void Log::error(stringstream& message)
{
    CHECK_IF_INIT();
    ms_singleton->logError(message.str().c_str());
}
