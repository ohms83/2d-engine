//
//  IniParser.cpp
//  BomberMan
//
//  Created by nuttachai on 12/2/21.
//

#include "IniParser.hpp"

#include "engine/util/TypeCheck.h"

#include <fstream>

#ifdef USE_CC_FILE_UTILS
    #include "cocos2d.h"
#endif

using namespace std;
using namespace serialize;
using namespace util;
using namespace debug;

const string IniParser::DELIMITER = "=";
const string IniParser::COMMENT = "#;";
const IniParser::ValueMap IniParser::EMPTY_VALUE;

IniParser::IniParser(const std::string& filePath, bool caseSensitive)
: m_caseSensitive(caseSensitive)
{
    read(filePath);
}

bool IniParser::parse(const std::string& configString)
{
    stringstream ss;
    ss << configString;
    
    CHECK_IF_RETURN_MSG(!parse(ss), false, "Parsing error CONFIG_STR=%s", configString.c_str());
    return true;
}

bool IniParser::parse(std::istream& inputStream)
{
    string line;
    string category;
    
    while (std::getline(inputStream, line))
    {
        const size_t commentIndex = line.find_first_of(COMMENT);
        if (commentIndex != string::npos)
        {
            // The remaining of this line will be consider a part of comment.
            line = line.substr(0, commentIndex);
        }
        
        line = strutil::trim(line);
        
        if (line.empty()) {
            continue;
        }
        
        const size_t lastIndex = line.size() - 1;
        CHECK_IF_RETURN_MSG(lastIndex <= 1, false, "Parsing error LINE=%s REASON=%s", line.c_str(), "Invalid syntax");
        
        if (line.at(0) == '[' and line.at(lastIndex) == ']')
        {
            category = line.substr(1, lastIndex - 1);
        }
        else
        {
            CHECK_IF_RETURN_MSG(category.empty(), false, "Parsing error LINE=%s REASON=%s", line.c_str(), "Category is empty");
            
            vector<string> keyValue = strutil::split(line, DELIMITER.c_str());
            CHECK_IF_RETURN_MSG(keyValue.size() != 2, false, "Parsing error LINE=%s REASON=%s", line.c_str(), "Invalid config key/value");
            
            set(category, keyValue[0], util::strutil::trimLeft(keyValue[1]));
        }
    }
    return true;
}

bool IniParser::read(const std::string& filePath)
{
#ifdef USE_CC_FILE_UTILS
    std::string fileData = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);
    CHECK_IF_RETURN_MSG(!parse(fileData), false, "Error parsing file FILE=%s REASON=Parsing error", filePath.c_str());
#else
    ifstream infile(filePath);
    CHECK_IF_RETURN_MSG(!infile.good(), false, "Error opening file FILE=%s REASON=Invalid file path", filePath.c_str());
    CHECK_IF_RETURN_MSG(!parse(infile), false, "Error parsing file FILE=%s REASON=Parsing error", filePath.c_str());
#endif
    return true;
}

bool IniParser::write(const std::string& filePath) const
{
#ifdef USE_CC_FILE_UTILS
    stringstream outStream;
#else
    ofstream outStream(filePath);
    CHECK_IF_RETURN_MSG(!outStream.good(), false, "Error writing to file. FILE=%s REASON=Invalid file path", filePath.c_str());
#endif
    
    // TODO: Beautification
    for (const auto& categoryConfig : m_categories)
    {
        const string& category = categoryConfig.first;
        outStream << '[' << category << ']' << std::endl;
        
        for (const auto& keyValue : categoryConfig.second)
        {
            outStream << keyValue.first << DELIMITER << keyValue.second << std::endl;
        }
    }

#ifdef USE_CC_FILE_UTILS
    CHECK_IF_RETURN_MSG(
        cocos2d::FileUtils::getInstance()->writeStringToFile(outStream.str(), filePath),
        false,
        "Error writing to file. FILE=%s REASON=Invalid file path", filePath.c_str()
    );
#endif
    
    return true;
}

const IniParser::ValueMap& IniParser::getValues(const std::string& category) const
{
    std::string cat = util::strutil::trim(category);
    if (!m_caseSensitive) {
        util::strutil::tolower(cat);
    }

    const auto& categoryItr = m_categories.find(cat);

    if(categoryItr == m_categories.end()) {
        LOG_WARNING("ERROR=Invalid category CATEGORY=%s", category.c_str());
        return EMPTY_VALUE;
    }
    return categoryItr->second;
}
