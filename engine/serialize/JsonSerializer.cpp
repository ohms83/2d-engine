#include "JsonSerializer.hpp"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include <sstream>

#include "../debug/DebugMacros.h"

using namespace std;
using namespace serialize;

namespace
{
    Serializable fromJson(const rapidjson::Value& value)
    {
        if(value.IsNumber())
        {
            if(value.IsInt() || value.IsInt64())
            {
				return Serializable((int64_t)value.GetInt64());
            }
            else if(value.IsUint() || value.IsUint64())
            {
				return Serializable((uint64_t)value.GetUint64());
            }
            else if(value.IsFloat() || value.IsDouble())
            {
				return Serializable((double)value.GetDouble());
            }
            else
            {
                ALERT_MSG(false, "Unable to determin JSON type");
            }
        }
        else if(value.IsBool())
        {
            return Serializable(value.GetBool());
        }
		else if (value.IsString())
		{
			return Serializable(string(value.GetString()));
		}
        else if(value.IsArray())
        {
            const auto& array = value.GetArray();
            Serializable::Array serializeArray;
            
            for( const auto& elem : array )
            {
                serializeArray.emplace_back(fromJson(elem));
            }
            
            return serializeArray;
        }
        else if(value.IsObject())
        {
            const auto& map = value.GetObject();
            Serializable::Map serializeMap;
            
            for(const auto& elem : map)
            {
                pair<string, Serializable> mapEntry = {
                    elem.name.GetString(),
                    fromJson(elem.value)
                };
                serializeMap.insert(mapEntry);
            }
            
            return serializeMap;
        }

        return Serializable();
    }
    
    void writeJson(const Serializable& source, rapidjson::Writer<rapidjson::StringBuffer>& writer)
    {
        switch (source.getType())
        {
            case Serializable::Type::INTEGER:
                writer.Int64(source.getInt());
                break;

			case Serializable::Type::UNSIGNED:
				writer.Uint64(source.getUint());
				break;

            case Serializable::Type::FLOAT:
                writer.Double(source.getFloat());
                break;

            case Serializable::Type::BOOLEAN:
                writer.Bool(source.getBoolean());
                break;

            case Serializable::Type::STRING:
                writer.String(source.getString().c_str());
                break;

            case Serializable::Type::ARRAY:
            {
                Serializable::Array array = source.getArray();
                
                writer.StartArray();
                for( const auto& elem : array )
                {
                    writeJson(elem, writer);
                }
                writer.EndArray();
            }
                break;

            case Serializable::Type::MAP:
            {
                Serializable::Map map = source.getMap();
                
                writer.StartObject();
                for( const auto& elem : map )
                {
                    writer.Key(elem.first.c_str());
                    writeJson(elem.second, writer);
                }
                writer.EndObject();
            }
                break;

            default:
                ALERT_MSG(false, "Unknown Serializable::Type(%d)", source.getType());
                break;
        }
    }
}

ostream& JsonSerializer::pack(const Serializable& source, ostream& outStream) const
{
    string jsonString = toString(source);
    outStream << jsonString;
    return outStream;
}

Serializable JsonSerializer::unpack(istream& inStream) const
{
    string contents;
    inStream.seekg(0, std::ios::end);
    contents.resize(inStream.tellg());
    inStream.seekg(0, std::ios::beg);
    inStream.read(&contents[0], contents.size());
    return fromString(contents);
}

string JsonSerializer::toString(const Serializable& source) const
{
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    writeJson(source, writer);
    return s.GetString();
}

Serializable JsonSerializer::fromString(const string& inString) const
{
    rapidjson::Document document;
    document.Parse(inString.c_str());

    if(rapidjson::kParseErrorNone == document.GetParseError())
    {
        return fromJson(document);
    }
    else
    {
        CCLOGWARN("ERROR: Invalid JSON -- %s", inString.c_str());
        return Serializable();
    }
}
