//
//  StringExt.h
//  MiniRpg
//
//  A file containing collections of string utility functions.
//
//  Created by nuttachai.tipprasert on 2016/08/18.
//
//

#ifndef __StringExt_hpp__
#define __StringExt_hpp__

#include <map>
#include <string>
#include <sstream>
#include <vector>

namespace util
{
    namespace strutil
    {
        //! Returns a copy of @c original with all of the left most characters in @c delimeters being removed.
         std::string trimLeft( const std::string& original, const std::string& delimiters = " \f\n\r\t\v" );
        //! Returns a copy of @c original with all of the right most characters in @c delimeters being removed.
         std::string trimRight( const std::string& original, const std::string& delimiters = " \f\n\r\t\v" );
        //! Returns a copy of @c original with all of the left and right most characters in @c delimeters being removed.
         std::string trim( const std::string& original, const std::string& delimiters = " \f\n\r\t\v" );
        /**
         *	Find and replace portions of the given string with the new sub string. If the sub-string to be replaced cannot be found,
         *	the source string will be kept unchange.
         *	@return A constant reference to the source string.
         */
        const std::string& replace( std::string& src, const std::string& subStr, const std::string& newSubStr );
        /**
         *	Find and replace portions of the given string with the new sub string. If the sub-string to be replaced cannot be found,
         *	the source string will be kept unchange.
         *	@return A copy of @c src which the target @c subStr being replaced.
         */
        const std::string replace( const char* src, const std::string& subStr, const std::string& newSubStr );
        /**
         *  Find and replace all occurences of the given string with the new sub string. If the sub-string to be replaced cannot be found,
         *  the source string will be kept unchange.
         *  @return A constant reference to the source string.
         */
        const std::string& replaceAll( std::string& src, const std::string& subStr, const std::string& newSubStr );
        /**
         *  Find and replace all occurences of the given string with the new sub string. If the sub-string to be replaced cannot be found,
         *  the source string will be kept unchange.
         *  @return A copy of @c src which the target @c subStr being replaced.
         */
        const std::string replaceAll( const char* src, const std::string& subStr, const std::string& newSubStr );
        /**
         *  Find and replace all occurences of the given string keys with their corresponding new sub string in the replaceMapping.
         *  If the sub-string to be replaced cannot be found, the source string will be kept unchange.
         *  @return A constant reference to the source string.
        */
        const std::string& replaceAll( std::string& src, const std::map<const std::string, const std::string>& replaceMapping );
        /**
         *  Find and replace all occurences of the given string keys with their corresponding new sub string in the replaceMapping.
         *  If the sub-string to be replaced cannot be found, the source string will be kept unchange.
         *  @return A copy of @c src which the keys specified in @c replaceMapping being replaced.
        */
        const std::string replaceAll( const char* src, const std::map<const std::string, const std::string>& replaceMapping );
        
        /**
         *  Transform all characters in the given string into lower case.
         *  @return A constant reference to the modified source string.
         */
        const std::string& tolower( std::string& str );
        /**
         *  Make a copy of the given string which all the characters are in lower case.
         *  @return A copy of @c src which all the characters are in lower case.
         */
        const std::string tolower( const std::string& str );
        /**
         *  Transform all characters in the given string into upper case.
         *  @return A constant reference to the modified source string.
         */
        const std::string& toupper( std::string& str );
        /**
         *  Make a copy of the given string which all the characters are in upper case.
         *  @return A copy of @c src which all the characters are in upper case.
         */
        const std::string toupper( const std::string& str );
        
        /**
         *  Parse the given string and return its value in specified type.
         *  @param outValue An in/out parameter containing parsing result. In case of parsing error, this parameter won't be altered.
         *  @return @c true if there's no parsing error; otherwise, @c false
         */
        template<typename T>
        bool parse(const std::string& str, T& outValue)
        {
            try {
                std::stringstream ss;
                ss << str;
                ss >> outValue;
                return true;
            }
            catch(std::exception e) {
                return false;
            }
        }
        
        //! Splits string into tokens.
        std::vector<std::string> split(const std::string& str, const char* delimeters);
    }
}

#endif /* __StringExt_hpp__ */
