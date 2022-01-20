#pragma once
#include "engine/debug/Log.hpp"
#include "engine/platform/PlatformMacros.h"

/**
 *  If the @c _condition is @c true, the caller function will be immediately returned.
 */
#define CHECK_IF(_condition) \
if(_condition) { \
    LOG_ERROR("%s", #_condition); \
    return; \
}

/**
 *  If the @c _condition is @c true, the caller function will be immediately returned.
 */
#define CHECK_IF_MSG(_condition, fmt, ...) \
if(_condition) { \
    LOG_ERROR("%s. " fmt, #_condition, ##__VA_ARGS__); \
    return; \
}

/**
 *  If the @c _condition is @c true, the caller function will be immediately returned with @c _returnValue.
 */
#define CHECK_IF_RETURN(_condition, _returnValue) \
if(_condition) { \
    LOG_ERROR("%s", #_condition); \
    return _returnValue; \
}

/**
 *  If the @c _condition is @c true, the caller function will be immediately returned with @c _returnValue.
 */
#define CHECK_IF_RETURN_MSG(_condition, _returnValue, fmt, ...) \
if(_condition) { \
    LOG_ERROR("%s. " fmt, #_condition, ##__VA_ARGS__); \
    return _returnValue; \
}

/**
 *  If the @c _pointer is null, the caller function will be immediately returned.
 */
#define CHECK_IF_NULL(_pointer) \
if(_pointer == nullptr) { \
    LOG_ERROR("%s == nullptr", #_pointer); \
    return; \
}

/**
 *  If the @c _pointer is null, the caller function will be immediately returned.
 */
#define CHECK_IF_NULL_MSG(_pointer, fmt, ...) \
if(_pointer == nullptr) { \
    LOG_ERROR("%s == nullptr. " fmt, #_pointer, ##__VA_ARGS__); \
    return; \
}

/**
 *  If the @c _pointer is null, the caller function will be immediately returned with @c _returnValue .
 */
#define CHECK_IF_NULL_RETURN(_pointer, _returnValue) \
if(_pointer == nullptr) { \
    LOG_ERROR("%s == nullptr", #_pointer); \
    return _returnValue; \
}

/**
 *  If the @c _pointer is null, the caller function will be immediately returned with @c _returnValue .
 */
#define CHECK_IF_NULL_RETURN_MSG(_pointer, _returnValue, fmt, ...) \
if(_pointer == nullptr) { \
    LOG_ERROR("%s == nullptr. " fmt, #_pointer, ##__VA_ARGS__); \
    return _returnValue; \
}

// Check whether the given _index is out of _array's range (_max). If it is, the caller function
// will be immediately returned.
#define CHECK_ARRAY_INDEX(_array, _index, _max) \
if(_index < 0 || _index >= _max) { \
    LOG_ERROR("Array index out of bound. %s[%d/%d]", #_array, _index, _max); \
    return; \
}

// Check whether the given _index is out of _array's range (_max). If it is, the caller function
// will be immediately returned.
#define CHECK_ARRAY_INDEX_MSG(_array, _index, _max, fmt, ...) \
if(_index < 0 || _index >= _max) { \
    LOG_ERROR("Array index out of bound. %s[%d/%d]. " fmt, #_array, _index, _max, ##__VA_ARGS__); \
    return; \
}

// Check whether the given _index is out of _array's range (_max). If it is, the caller function
// will be immediately returned with the _returnValue.
#define CHECK_ARRAY_INDEX_RETURN(_array, _index, _max, _returnValue) \
if(_index < 0 || _index >= _max) { \
    LOG_ERROR("Array index out of bound. %s[%d/%d]", #_array, _index, _max); \
    return _returnValue; \
}

// Check whether the given _index is out of _array's range (_max). If it is, the caller function
// will be immediately returned with the _returnValue.
#define CHECK_ARRAY_INDEX_RETURN_MSG(_array, _index, _max, _returnValue, fmt, ...) \
if(_index < 0 || _index >= _max) { \
    LOG_ERROR("Array index out of bound. %s[%d/%d]. " fmt, #_array, _index, _max, ##__VA_ARGS__); \
    return _returnValue; \
}

// Check whether the given _index is out of _array's range (_array.size()).
// If it is, the caller function will be immediately returned.
#define CHECK_CONTAINER_INDEX(_array, _index) \
if(_index < 0 || _index >= _array.size()) { \
    LOG_ERROR("Array index out of bound. %s[%d/%d]", #_array, _index, _array.size()); \
    return; \
}

// Check whether the given _index is out of _array's range (_array.size()).
// If it is, the caller function will be immediately returned.
#define CHECK_CONTAINER_INDEX_MSG(_array, _index, fmt, ...) \
if(_index < 0 || _index >= _array.size()) { \
    LOG_ERROR("Array index out of bound. %s[%d/%d]." fmt, #_array, _index, _array.size(), ##__VA_ARGS__); \
    return; \
}

// Check whether the given _index is out of _array's range (_array.size()).
// If it is, the caller functionwill be immediately returned with the _returnValue.
#define CHECK_CONTAINER_INDEX_RETURN(_array, _index, _returnValue) \
if(_index < 0 || _index >= _array.size()) { \
    LOG_ERROR("Array index out of bound. %s[%d/%d]", #_array, _index, _array.size()); \
    return _returnValue; \
}

// Check whether the given _index is out of _array's range (_array.size()).
// If it is, the caller functionwill be immediately returned with the _returnValue.
#define CHECK_CONTAINER_INDEX_RETURN_MSG(_array, _index, _returnValue, fmt, ...) \
if(_index < 0 || _index >= _array.size()) { \
    LOG_ERROR("Array index out of bound. %s[%d/%d]." fmt, #_array, _index, _array.size(), ##__VA_ARGS__; \
    return _returnValue; \
}
