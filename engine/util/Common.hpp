#ifndef Utils_hpp
#define Utils_hpp

#include <string>
#include <functional>
#include <vector>

#include <type_traits>
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <cxxabi.h>
#endif

namespace util
{
    namespace common
    {
        template<typename _T>
        const std::string getClassName( _T &instance )
        {
            std::string outClassName;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
            int status = 0;
            char* typeName = abi::__cxa_demangle(typeid(instance).name(), 0, 0, &status);
            if( typeName )
            {
                outClassName.assign( typeName );
                free( typeName );
            }
#else
            const std::type_info& id = typeid(instance);
            const char* typeName = id.name();
            if( typeName )
            {
                outClassName.assign( typeName );
            }
#endif
            
            return outClassName;
        }
        
        /** A helper function to run multiple test cases at the same time.
         *  To run the test cases that require arguments, plese wrap those test cases and
         *  their arguments together using std::bind().
         *  Example, runTest(std::bind(test1, arg1, arg2), std::bind(test2, arg3));
         */
        bool runTest(std::vector<std::function<bool(void)>> testCases);
    }
}

#endif /* Utils_hpp */
