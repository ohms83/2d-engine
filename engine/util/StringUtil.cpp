//
//  StringExt.cpp
//  MiniRPG
//
//  Created by nuttachai.tipprasert on 2016/08/18.
//
//

#include "StringUtil.hpp"
#include <locale>
#include <algorithm>

#include "debug/Log.hpp"

using namespace std;

string util::strutil::trimLeft( const string& original, const string& delimiters )
{
	string result = original;
	
	try
	{
		size_t index = original.find_first_not_of( delimiters );
		
		if( index > 0 && index < original.size() )
		{
			result = original.substr( index );
		}
	}
	catch( const exception& e )
	{
		LOG_ERROR( "Unhandle exception: %s", e.what() );
	}
	
	return result;
}

string util::strutil::trimRight( const string& original, const string& delimiters )
{
	string result = original;
	
	try
	{
		size_t index = original.find_last_not_of( delimiters ) + 1;
		
		if( index < original.size() )
		{
			result = original.substr( 0, index );
		}
	}
	catch( const exception& e )
	{
        LOG_ERROR( "Unhandle exception: %s", e.what() );
	}
	
	return result;
}

string util::strutil::trim( const string& original, const string& delimiters )
{
	string result = original;
	
	try
	{
		result = trimLeft( trimRight( original, delimiters ), delimiters );
	}
	catch( const exception& e )
	{
        LOG_ERROR( "Unhandle exception: %s", e.what() );
	}
	
	return result;
}

const string& util::strutil::replace( string& src, const string& subStr, const string& newSubStr )
{
	const size_t index = src.find( subStr );
	if( index != src.npos )
	{
		src.replace( index, subStr.size(), newSubStr );
	}
	
	return src;
}

const string util::strutil::replace( const char* src, const string& subStr, const string& newSubStr )
{
	string strSrc( src );
    return util::strutil::replace( strSrc, subStr, newSubStr );
}

const string& util::strutil::replaceAll( string& src, const string& subStr, const string& newSubStr )
{
    size_t index = src.find( subStr );
    while( index != src.npos )
    {
        src.replace( index, subStr.size(), newSubStr );
        index = src.find( subStr, index + newSubStr.size() );
    }
    
    return src;
}

const string util::strutil::replaceAll( const char* src, const string& subStr, const string& newSubStr )
{
    string strSrc( src );    
    return util::strutil::replaceAll( strSrc, subStr, newSubStr );
}

const string& util::strutil::replaceAll( string& src, const map<const string, const string>& replaceMapping )
{
    for( auto & kvp : replaceMapping )
    {
        util::strutil::replaceAll( src, kvp.first, kvp.second );
    }
    return src;
}

const string util::strutil::replaceAll( const char* src, const map<const string, const string>& replaceMapping )
{
    string strSrc( src );
    return util::strutil::replaceAll( strSrc, replaceMapping );
}

const string& util::strutil::tolower( string& str )
{
    transform( str.begin(), str.end(), str.begin(), ::tolower );
    return str;
}

const string util::strutil::tolower( const string& str )
{
    string copy = str;
    util::strutil::tolower( copy );
    return copy;
}

const string& util::strutil::toupper( string& str )
{
    transform( str.begin(), str.end(), str.begin(), ::toupper );
    return str;
}

const string util::strutil::toupper( const string& str )
{
    string copy = str;
    util::strutil::toupper( copy );
    return copy;
}

vector<string> util::strutil::split(const string& str, const char* delimeters)
{
    vector<string> tokens;
    
    if (!str.empty() && delimeters != nullptr)
    {
        size_t bufSize = str.size() + 1;
        char* tmpBuf = new char[str.size() + 1];
        memcpy(tmpBuf, str.c_str(), bufSize);
        tmpBuf[bufSize - 1] = '\0';
        
        const char* substr = strtok(tmpBuf, delimeters);
        while (substr)
        {
            tokens.emplace_back(substr);
            substr = strtok(nullptr, delimeters);
        }
        
        delete [] tmpBuf;
    }
    
    return tokens;
}
