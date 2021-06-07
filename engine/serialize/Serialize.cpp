//
//  Serialize.cpp
//  MiniRPG
//
//  Created by nuttachai on 23/10/19.
//

#include "Serialize.hpp"
#include "engine/debug/Log.hpp"

#include <sstream>

using namespace serialize;
using namespace std;

#define DECLARE_SERIALIZE_VALUE(className, type, overridingMethod) \
class className final : public SerializeValue \
{ \
public: \
	className(const type& value) : m_value(value) {} \
	className(type&& value) : m_value(move(value)) {} \
	const type& overridingMethod() const override { return m_value; } \
private: \
	type m_value; \
};

DECLARE_SERIALIZE_VALUE(SerializeValueString, string, getString)
DECLARE_SERIALIZE_VALUE(SerializeValueArray, Serializable::Array, getArray)
DECLARE_SERIALIZE_VALUE(SerializeValueMap, Serializable::Map, getMap)

class SerializeInt final : public SerializeValue
{
public:
    SerializeInt(int64_t value) : m_value(value) {}

	int64_t getInt() const override { return m_value; }
	uint64_t getUint() const override { return (uint64_t)m_value; }
    double getFloat() const override { return (double)m_value; }
    bool getBoolean() const override { return (bool)m_value; }
private:
	int64_t m_value;
};

class SerializeUint final : public SerializeValue
{
public:
	SerializeUint(uint64_t value) : m_value(value) {}

	int64_t getInt() const override { return (int64_t)m_value; }
	uint64_t getUint() const override { return m_value; }
	double getFloat() const override { return (double)m_value; }
	bool getBoolean() const override { return (bool)m_value; }
private:
	uint64_t m_value;
};

class SerializeFloat final : public SerializeValue
{
public:
    SerializeFloat(double value) : m_value(value) {}

	int64_t getInt() const override { return (int64_t)m_value; }
	uint64_t getUint() const override { return (uint64_t)m_value; }
    double getFloat() const override { return m_value; }
    bool getBoolean() const override { return (bool)m_value; }
private:
    double m_value;
};

class SerializeValueBoolean final : public SerializeValue
{
public:
    SerializeValueBoolean(bool value) : m_value(value) {}

	int64_t getInt() const override { return (int64_t)m_value; }
	uint64_t getUint() const override { return (uint64_t)m_value; }
    double getFloat() const override { return (double)m_value; }
    bool getBoolean() const override { return m_value; }
private:
    bool m_value;
};

Serializable::Serializable()
	: m_type(Type::NONE)
{
    m_pValue.reset(new SerializeValue());
}

Serializable::Serializable(const Serializable& rhs)
	: m_type(rhs.getType())
{
	copyValue(rhs.getType(), *rhs.m_pValue);
}

Serializable::Serializable(Serializable&& rhs) noexcept
{
	m_type = rhs.getType();
	m_pValue = std::move(rhs.m_pValue);
}

Serializable::Serializable(uint8_t uintValue)
	: m_type(Type::UNSIGNED)
{
	m_pValue.reset(new SerializeUint(uintValue));
}
Serializable::Serializable(uint16_t uintValue)
	: m_type(Type::UNSIGNED)
{
	m_pValue.reset(new SerializeUint(uintValue));
}
Serializable::Serializable(uint32_t uintValue)
	: m_type(Type::UNSIGNED)
{
	m_pValue.reset(new SerializeUint(uintValue));
}
Serializable::Serializable(uint64_t uintValue)
	: m_type(Type::UNSIGNED)
{
	m_pValue.reset(new SerializeUint(uintValue));
}
Serializable::Serializable(int8_t intValue)
	: m_type(Type::INTEGER)
{
	m_pValue.reset(new SerializeInt(intValue));
}
Serializable::Serializable(int16_t intValue)
	: m_type(Type::INTEGER)
{
	m_pValue.reset(new SerializeInt(intValue));
}
Serializable::Serializable(int32_t intValue)
	: m_type(Type::INTEGER)
{
	m_pValue.reset(new SerializeInt(intValue));
}

Serializable::Serializable(int64_t intValue)
	: m_type(Type::INTEGER)
{
	m_pValue.reset(new SerializeInt(intValue));
}

Serializable::Serializable(double floatValue)
	: m_type(Type::FLOAT)
{
	m_pValue.reset(new SerializeFloat(floatValue));
}
Serializable::Serializable(bool boolValue)
	: m_type(Type::BOOLEAN)
{
	m_pValue.reset(new SerializeValueBoolean(boolValue));
}
Serializable::Serializable(const std::string& stringValue)
	: m_type(Type::STRING)
{
	m_pValue.reset(new SerializeValueString(stringValue));
}
Serializable::Serializable(const Serializable::Array& arrayValue)
	: m_type(Type::ARRAY)
{
	m_pValue.reset(new SerializeValueArray(arrayValue));
}
Serializable::Serializable(const Serializable::Map& mapValue)
	: m_type(Type::MAP)
{
	m_pValue.reset(new SerializeValueMap(mapValue));
}
Serializable::Serializable(std::string&& stringValue)
    : m_type(Type::STRING)
{
    m_pValue.reset(new SerializeValueString(move(stringValue)));
}
Serializable::Serializable(Serializable::Array&& arrayValue)
    : m_type(Type::ARRAY)
{
    m_pValue.reset(new SerializeValueArray(move(arrayValue)));
}
Serializable::Serializable(Serializable::Map&& mapValue)
    : m_type(Type::MAP)
{
    m_pValue.reset(new SerializeValueMap(move(mapValue)));
}

Serializable::operator int64_t() const
{
	return getInt();
}
Serializable::operator uint64_t() const
{
	return getUint();
}
Serializable::operator double() const
{
	return getFloat();
}
Serializable::operator bool() const
{
	return getBoolean();
}
Serializable::operator std::string() const
{
	return getString();
}
Serializable::operator Array() const
{
	return getArray();
}
Serializable::operator Map() const
{
	return getMap();
}

Serializable& Serializable::operator = (Serializable&& rhs) noexcept
{
    m_type = rhs.getType();
    m_pValue = std::move(rhs.m_pValue);
    return *this;
}

bool Serializable::operator == (const Serializable& rhs) const
{
    switch (getType())
    {
        case Type::INTEGER:
            return getInt() == rhs.getInt();
        case Type::UNSIGNED:
            return getUint() == rhs.getUint();
        case Type::FLOAT:
            return getFloat() == rhs.getFloat();
        case Type::BOOLEAN:
            return getBoolean() == rhs.getBoolean();
        case Type::STRING:
            return getString() == rhs.getString();
        case Type::ARRAY:
        {
            const auto& arrayValue = getArray();
            const auto& rhsArrayValue = rhs.getArray();
            const size_t size = arrayValue.size();
            
            if (size != rhsArrayValue.size()) {
                return false;
            }
            
            for (size_t i = 0; i < size; ++i)
            {
                if (arrayValue.at(i) != rhsArrayValue.at(i)) {
                    return false;
                }
            }
            return true;
        }
        case Type::MAP:
        {
            const auto& mapValue = getMap();
            const auto& rhsMapValue = rhs.getMap();
            const size_t size = mapValue.size();
            const auto& rhsMapEnd = rhsMapValue.end();
            
            if (size != rhsMapValue.size()) {
                return false;
            }
            
            for (const auto& elem : mapValue)
            {
                const auto& rhsElemItr = rhsMapValue.find(elem.first);
                if (rhsElemItr == rhsMapEnd) {
                    return false;
                }
                if (rhsElemItr->second != elem.second) {
                    return false;
                }
            }
            return true;
        }
        default:
            break;
    }
    
    return false;
}

bool Serializable::operator != (const Serializable& rhs) const
{
    return !(*this == rhs);
}

const Serializable& Serializable::operator [] (const string& key) const
{
    LOG_ASSERT(getType() == Type::MAP, "Object is not a map TYPE=", getType());
    return m_pValue->getMap().at(key);
}

const Serializable& Serializable::at(const string& key) const
{
    LOG_ASSERT(getType() == Type::MAP, "Object is not a map TYPE=", getType());
    return m_pValue->getMap().at(key);
}

const Serializable& Serializable::operator [] (size_t index) const
{
    LOG_ASSERT(getType() == Type::ARRAY, "Object is not an array TYPE=", getType());
    return m_pValue->getArray()[index];
}

const Serializable& Serializable::at(size_t index) const
{
    LOG_ASSERT(getType() == Type::ARRAY, "Object is not an array TYPE=", getType());
    return m_pValue->getArray()[index];
}

bool Serializable::hasKey(const std::string& key) const
{
    const Map& mapObj = getMap();
    return mapObj.find(key) != mapObj.end();
}

bool Serializable::tryGet(const std::string& key, Serializable& outValue) const
{
    const Map& mapObj = getMap();
    auto itr = mapObj.find(key);
    
    if (itr != mapObj.end()) {
        outValue.copyValue(itr->second.m_type, *itr->second.m_pValue);
        return true;
    }
    return false;
}

int64_t Serializable::getInt() const
{
	return m_pValue->getInt();
}
uint64_t Serializable::getUint() const
{
	return m_pValue->getUint();
}
double Serializable::getFloat() const
{
	return m_pValue->getFloat();
}
bool Serializable::getBoolean() const
{
	return m_pValue->getBoolean();
}
const std::string& Serializable::getString() const
{
	return m_pValue->getString();
}
const Serializable::Array& Serializable::getArray() const
{
	return m_pValue->getArray();
}
const Serializable::Map& Serializable::getMap() const
{
	return m_pValue->getMap();
}

void Serializable::copyValue(Serializable::Type type, const SerializeValue& value)
{
	switch (type)
	{
        case Type::INTEGER:
            m_pValue.reset(new SerializeInt(value.getInt()));
            break;
        case Type::UNSIGNED:
            m_pValue.reset(new SerializeUint(value.getUint()));
            break;
        case Type::FLOAT:
            m_pValue.reset(new SerializeFloat(value.getFloat()));
            break;
        case Type::BOOLEAN:
            m_pValue.reset(new SerializeValueBoolean(value.getBoolean()));
            break;
        case Type::STRING:
            m_pValue.reset(new SerializeValueString(value.getString()));
            break;
        case Type::ARRAY:
            m_pValue.reset(new SerializeValueArray(value.getArray()));
            break;
        case Type::MAP:
            m_pValue.reset(new SerializeValueMap(value.getMap()));
            break;
        default:
            m_type = Type::NONE;
            m_pValue.reset(new SerializeValue());
            break;
	}
}

string Serializable::getTypeString(Type type)
{
    switch (type)
    {
        case Type::INTEGER:
            return "Serializable::Type::INTEGER";
        case Type::UNSIGNED:
            return "Serializable::Type::UNSIGNED";
        case Type::FLOAT:
            return "Serializable::Type::FLOAT";
        case Type::BOOLEAN:
            return "Serializable::Type::BOOLEAN";
        case Type::STRING:
            return "Serializable::Type::STRING";
        case Type::ARRAY:
            return "Serializable::Type::ARRAY";
        case Type::MAP:
            return "Serializable::Type::MAP";
        default:
            break;
    }
    return "Serializable::Type::NONE";
}
