//
//  IniParser.cpp
//  BomberMan
//
//  Created by nuttachai on 12/2/21.
//

#include "Config.hpp"

#include "engine/util/TypeCheck.h"

#include <fstream>

using namespace std;
using namespace serialize;
using namespace util;
using namespace debug;

const string Config::DELIMITER = "=";
const string Config::COMMENT = "#;";
const Config::ValueMap Config::EMPTY_VALUE;

Config::Config(const std::string& filePath, bool caseSensitive)
: m_caseSensitive(caseSensitive)
{
    read(filePath);
}

bool Config::parse(const std::string& configString)
{
    stringstream ss;
    ss << configString;
    
    CHECK_IF_RETURN_MSG(!parse(ss), false, "Parsing error CONFIG_STR=%s", configString.c_str());
    return true;
}

bool Config::parse(std::istream& inputStream)
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

bool Config::read(const std::string& filePath)
{
    ifstream infile(filePath);
    CHECK_IF_RETURN_MSG(!infile.good(), false, "Error opening file FILE=%s REASON=Invalid file path", filePath.c_str());
    CHECK_IF_RETURN_MSG(!parse(infile), false, "Error parsing file FILE=%s REASON=Parsing error", filePath.c_str());
    return true;
}

bool Config::write(const std::string& filePath) const
{
    ofstream outfile(filePath);
    CHECK_IF_RETURN_MSG(!outfile.good(), false, "Error writing to file. FILE=%s REASON=Invalid file path", filePath.c_str());
    
    // TODO: Beautification
    for (const auto& categoryConfig : m_categories)
    {
        const string& category = categoryConfig.first;
        outfile << '[' << category << ']' << std::endl;
        
        for (const auto& keyValue : categoryConfig.second)
        {
            outfile << keyValue.first << DELIMITER << keyValue.second << std::endl;
        }
    }
    
    return true;
}

const Config::ValueMap& Config::getValues(const std::string& category) const
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
