//
//  Serialize.hpp
//  MiniRPG
//
//  Created by nuttachai on 23/10/19.
//

#ifndef Serialize_hpp
#define Serialize_hpp

#include <ios>
#include <string>
#include <vector>
#include <map>
#include <assert.h>

// To make a class serializable, just use this macro durint the class declaration.
#define DECLARE_SERIALIZABLE(Type) \
public: Type(const serialize::Serializable& obj); \
public: operator serialize::Serializable () const;
// Stringtify the given param and insert it into specified serializeMap
#define PACK_MAP_PARAM(serializeMap, param) serializeMap.insert(std::pair<std::string, serialize::Serializable>(#param, param))
#define UNPACK_MAP_PARAM(serializeMap, param) param = serializeMap.at(#param)
// Unpack specified map param into int16_t.
#define UNPACK_MAP_PARAM_BYTE(serializeMap, param) param = ((int8_t)(int64_t)serializeMap.at(#param))
// Unpack specified map param into int16_t.
#define UNPACK_MAP_PARAM_SHORT(serializeMap, param) param = ((int16_t)(int64_t)serializeMap.at(#param))
// Unpack specified map param into int32_t.
#define UNPACK_MAP_PARAM_INT(serializeMap, param) param = ((int32_t)(int64_t)serializeMap.at(#param))
// Unpack specified map param into int64_t.
#define UNPACK_MAP_PARAM_LONG(serializeMap, param) param = (int64_t)serializeMap.at(#param)
// Unpack specified map param into int16_t.
#define UNPACK_MAP_PARAM_UBYTE(serializeMap, param) param = ((uint8_t)(int64_t)serializeMap.at(#param))
// Unpack specified map param into int16_t.
#define UNPACK_MAP_PARAM_USHORT(serializeMap, param) param = ((uint16_t)(int64_t)serializeMap.at(#param))
// Unpack specified map param into int32_t.
#define UNPACK_MAP_PARAM_UINT(serializeMap, param) param = ((uint32_t)(int64_t)serializeMap.at(#param))
// Unpack specified map param into int64_t.
#define UNPACK_MAP_PARAM_ULONG(serializeMap, param) param = (uint64_t)serializeMap.at(#param)
// Unpack specified map param into float.
#define UNPACK_MAP_PARAM_FLOAT(serializeMap, param) param = ((float)(double)serializeMap.at(#param))
// Unpack specified map param into double.
#define UNPACK_MAP_PARAM_DOUBLE(serializeMap, param) param = (double)serializeMap.at(#param)
// Unpack specified map param into bool.
#define UNPACK_MAP_PARAM_BOOL(serializeMap, param) param = (bool)serializeMap.at(#param)
#define UNPACK_MAP_PARAM_STRING(serializeMap, param) param = ((std::string)serializeMap.at(#param))

namespace serialize
{
	class SerializeValue;

	class Serializable final
	{
	public:
		enum class Type : int
		{
			NONE,
			INTEGER,
			UNSIGNED,
			FLOAT,
			BOOLEAN,
			STRING,
			ARRAY,
			MAP,
		};

		typedef std::vector<Serializable> Array;
		typedef std::map<std::string, Serializable> Map;

		Serializable();

		Serializable(const Serializable& rhs);
		
		// Move constructor
		Serializable(Serializable&& rhs) noexcept;

		Serializable(uint8_t uintValue);
		Serializable(uint16_t uintValue);
		Serializable(uint32_t uintValue);
		Serializable(uint64_t uintValue);
		Serializable(int16_t intValue);
		Serializable(int32_t intValue);
		Serializable(int64_t intValue);
		Serializable(int8_t uintValue);
		Serializable(double floatValue);
		Serializable(bool boolValue);
		Serializable(const std::string& stringValue);
		Serializable(const Array& arrayValue);
		Serializable(const Map& mapValue);
        Serializable(std::string&& stringValue);
        Serializable(Array&& arrayValue);
        Serializable(Map&& mapValue);
        
        virtual ~Serializable() {}

		operator int64_t() const;
		operator uint64_t() const;
		operator double() const;
		operator bool() const;
		operator std::string() const;
		operator Array() const;
		operator Map() const;
        
        Serializable& operator = (Serializable&& rhs) noexcept;
        
        bool operator == (const Serializable& rhs) const;
        bool operator != (const Serializable& rhs) const;

		int64_t getInt() const;
		uint64_t getUint() const;
		double getFloat() const;
		bool getBoolean() const;
		const std::string& getString() const;
		const Array& getArray() const;
		const Map& getMap() const;
        
        /// Convenient accessor for MAP type.
        const Serializable& operator [] (const std::string& key) const;
		/// Convenient accessor for MAP type.
		const Serializable& at(const std::string& key) const;
        /// Convenient accessor for ARRAY type.
        const Serializable& operator [] (size_t index) const;
		/// Convenient accessor for ARRAY type.
		const Serializable& at(size_t index) const;
        /**
         *  Check whether the specified @c key exist.
         *  @note This function will always return @c false for non Map type object.
         */
        bool hasKey(const std::string& key) const;
        /**
         *  Try to retrieve a @c Serializable object with the given @c key.
         *  @note If the object is a non-Map type, this function will always return @c false and @c outValue will be left unchanged.
         */
        bool tryGet(const std::string& key, Serializable& outValue) const;

        Type getType() const noexcept { return m_type; };
        
        /// Returns a string definition of the given type. This is mainly for debugging purpose.
        static std::string getTypeString(Type type);

	private:
		void copyValue(Type type, const SerializeValue& value);
		Type m_type = Type::NONE;
		std::unique_ptr<SerializeValue> m_pValue = nullptr;
	};

	class SerializeValue
	{
	public:
		virtual ~SerializeValue() {}
		virtual int64_t getInt() const
		{
			return 0;
		}
		virtual uint64_t getUint() const
		{
			return 0;
		}
		virtual double getFloat() const
		{
			return 0;
		}
		virtual bool getBoolean() const
		{
			return false;
		}
		virtual const std::string& getString() const
		{
			static std::string dummyString;
			return dummyString;
		}
		virtual const Serializable::Array& getArray() const
		{
			static Serializable::Array dummyArray;
			return dummyArray;
		}
		virtual const Serializable::Map& getMap() const
		{
			static Serializable::Map dummyMap;
			return dummyMap;
		}
	};

    template <class _DataType, class _SerializeType>
    Serializable::Array createSerializeArray(const std::vector<_DataType>& source)
    {
        Serializable::Array result;
        for (const _DataType& elem : source) {
            result.emplace_back(Serializable((_SerializeType)elem));
        }
        return result;
    }

    template <class _DataType, class _SerializeType>
    std::vector<_DataType> createArrayFromSerialize(const Serializable::Array& source)
    {
        std::vector<_DataType> result;
        for (const _DataType& elem : source) {
            result.emplace_back((_SerializeType)elem);
        }
        return result;
    }
    
    class Serializer
    {
    public:
        virtual ~Serializer() {}
        virtual std::ostream& pack(const Serializable& source, std::ostream& outStream) const = 0;
        virtual Serializable unpack(std::istream& inStream) const = 0;
    };
}

#endif /* Serialize_hpp */
