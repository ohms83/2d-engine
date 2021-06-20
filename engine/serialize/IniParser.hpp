//
//  IniParser.hpp
//  2D-Engine
//
//  Created by nuttachai on 12/2/21.
//

#ifndef IniParser_hpp
#define IniParser_hpp

#include <fstream>
#include <sstream>
#include <map>
#include <string>

#include "engine/util/StringUtil.hpp"
#include "engine/debug/Log.hpp"

namespace serialize
{
    /**
     *  @c IniParser class stores data in
     *  [Category]
     *  key=value
     *  format, where each line can only contains one key/value pair and both leading and
     *  traling whitespaces/tabs will be trimmed.
     *
     *  @note Both @c key and @c category are case insensitive. For example,
     *  [Control]
     *  MoveSpeed=10
     *  is equivalent to
     *  [control]
     *  movespeed=10
     */
    class IniParser final
    {
    public:
        typedef std::map<std::string, std::string> ValueMap;
        typedef std::map<std::string, ValueMap> CategoryMap;
        
        IniParser(bool caseSensitive = true) : m_caseSensitive(caseSensitive) {}
        //! Construct a @c IniParser object and read the config file from the specified @c filePath
        IniParser(const std::string& filePath, bool caseSensitive = false);
        
        //! Read config file from the specified @c filePath
        bool read(const std::string& filePath);
        //! Parse config from the specified the @c configString.
        bool parse(const std::string& configString);
        //! Parse config from the specified the @c inputStream.
        bool parse(std::istream& inputStream);
        //! Write current config to the specified @c filePath
        bool write(const std::string& filePath) const;
        
        /**
         *  Assign a @c value to the specified @c key in @c category .
         *  @note The new @c category will be created if it doesn't exist.
         */
        template<typename T>
        void set(const std::string& category, const std::string& key, T value)
        {
            std::stringstream ss;
            ss << value;
            
            std::string cat = util::strutil::trim(category);
            if (!m_caseSensitive) {
                util::strutil::tolower(cat);
            }

            std::string k = util::strutil::trim(key);
            if (!m_caseSensitive) {
                util::strutil::tolower(k);
            }

            ValueMap& values = m_categories[cat];
            values[k] = (m_caseSensitive ? ss.str() : util::strutil::tolower(ss.str()));
        }
        
        /**
         *  Retrieve the configuraton value @c key from @c category . @c defaultValue will be returned in case
         *  either @c key or @c category doesn't exist.
         */
        template<typename T>
        T get(const std::string& category, const std::string& key, T defaultValue) const
        {
            std::string cat = util::strutil::trim(category);
            if (!m_caseSensitive) {
                util::strutil::tolower(cat);
            }

            std::string k = util::strutil::trim(key);
            if (!m_caseSensitive) {
                util::strutil::tolower(k);
            }
            
            const auto& categoryItr = m_categories.find(cat);
            if(categoryItr == m_categories.end()) {
                LOG_WARNING("ERROR=Invalid category CATEGORY=%s", category.c_str());
                return defaultValue;
            }
            
            const ValueMap& valueMap = categoryItr->second;
            const auto& keyItr = valueMap.find(k);
            if(keyItr == valueMap.end()) {
                LOG_WARNING("ERROR=Invalid key KEY=%s", key.c_str());
                return defaultValue;
            }
            
            T ret;
            util::strutil::parse(keyItr->second, ret);
            return ret;
        }
        
        int getInt(const std::string& category, const std::string& key) const
        {
            return get<int>(category, key, 0);
        }
        
        bool getBool(const std::string& category, const std::string& key) const
        {
            return get<bool>(category, key, false);
        }
        
        float getFloat(const std::string& category, const std::string& key) const
        {
            return get<float>(category, key, 0.0f);
        }
        
        std::string getString(const std::string& category, const std::string& key) const
        {
            return get<std::string>(category, key, std::string());
        }

        /// Returns all values from the given category or an empty list; in case the specified category cannot be found.
        const ValueMap& getValues(const std::string& category) const;
        
        static const std::string DELIMITER;
        static const std::string COMMENT;
        
    private:
        bool m_caseSensitive = false;
        CategoryMap m_categories;
        /// Will be returned from @c getValues in case of error
        static const ValueMap EMPTY_VALUE;
    };
}

#endif /* IniParser_hpp */
