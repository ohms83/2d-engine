//
//  MsgpackSerializer.cpp
//  MiniRPG
//
//  Created by nuttachai on 16/11/19.
//

#include "MsgpackSerializer.hpp"
#include "msgpack.hpp"
#include "custom/custom.hpp" // Custom Messagepack adaptors
#include "debug/DebugMacros.h"

using namespace std;
using namespace serialize;

// msgpack adaptors
namespace msgpack {
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
        namespace adaptor {
            // Place class template specialization here
            template<>
            struct convert<Serializable> {
                msgpack::object const& operator()(msgpack::object const& o, Serializable& v) const {
                    // TODO:
                    switch (o.type)
                    {
                        case msgpack::v1::type::POSITIVE_INTEGER:
                            v = Serializable(o.as<uint64_t>());
                            break;

                        case msgpack::v1::type::NEGATIVE_INTEGER:
                            v = Serializable(o.as<int64_t>());
                            break;

                        case msgpack::v1::type::BOOLEAN:
                            v = Serializable(o.as<bool>());
                            break;

                        case msgpack::v1::type::FLOAT32:
                        case msgpack::v1::type::FLOAT64:
                            v = Serializable(o.as<double>());
                            break;

                        case msgpack::v1::type::STR:
                            v = Serializable(o.as<string>());
                            break;

                        case msgpack::v1::type::ARRAY:
                        {
                            Serializable::Array serializeArray;

                            for (const auto& element : o.as<msgpack::object_array>())
                            {
                                serializeArray.emplace_back(element.as<Serializable>());
                            }

                            v = std::move(serializeArray);
                            break;
                        }

                        case msgpack::v1::type::MAP:
                        {
                            Serializable::Map serializeMap;

                            for (const auto& kv : o.as<msgpack::object_map>())
                            {
                                pair<string, Serializable>  mapEntry = {
                                    kv.key.as<string>(),
                                    kv.val.as<Serializable>(),
                                };
                                serializeMap.insert(mapEntry);
                            }

                            v = std::move(serializeMap);
                            break;
                        }

                        default:
                            ALERT_MSG(false, "Unknown msgpack::object(%d)", o.type);
                            break;
                    }
                    return o;
                }
            };

            template<>
            struct pack<Serializable> {
                template <typename Stream>
                packer<Stream>& operator()(msgpack::packer<Stream>& o, Serializable const& source) const
                {
                    switch (source.getType())
                    {
                        case Serializable::Type::INTEGER:
                            o.pack<int64_t>(source.getInt());
                            break;

                        case Serializable::Type::UNSIGNED:
                            o.pack<uint64_t>(source.getUint());
                            break;

                        case Serializable::Type::FLOAT:
                            o.pack<double>(source.getFloat());
                            break;

                        case Serializable::Type::BOOLEAN:
                            o.pack<bool>(source.getBoolean());
                            break;

                        case Serializable::Type::STRING:
                            o.pack<string>(source.getString());
                            break;

                        case Serializable::Type::ARRAY:
                        {
                            Serializable::Array array = source.getArray();
                            o.pack_array((uint32_t)array.size());
                            
                            for (const auto& elem : array) {
                                o.pack(elem);
                            }
                        }
                        break;

                        case Serializable::Type::MAP:
                        {
                            Serializable::Map map = source.getMap();
                            o.pack_map((uint32_t)map.size());
                            
                            for (const auto& kv : map) {
                                o.pack(kv.first);
                                o.pack(kv.second);
                            }
                        }
                        break;

                        default:
                            ALERT_MSG(false, "Unknown Serializable::Type(%d)", source.getType());
                            break;
                    }
                    return o;
                }
            };

            template <>
            struct object_with_zone<Serializable> {
                void operator()(msgpack::object::with_zone& o, Serializable const& source) const
                {
                    switch (source.getType())
                    {
                        case Serializable::Type::INTEGER:
                            o.type = type::NEGATIVE_INTEGER;
                            o.via.i64 = source.getInt();
                            break;

                        case Serializable::Type::UNSIGNED:
                            o.type = type::POSITIVE_INTEGER;
                            o.via.u64 = source.getUint();
                            break;

                        case Serializable::Type::FLOAT:
                            o.type = type::FLOAT64;
                            o.via.f64 = source.getFloat();
                            break;

                        case Serializable::Type::BOOLEAN:
                            o.type = type::BOOLEAN;
                            o.via.boolean = source.getBoolean();
                            break;

                        case Serializable::Type::STRING:
                        {
                            const string& str = source.getString();
                            o.type = type::STR;
                            o.via.str.size = (uint32_t)str.size();
                            o.via.str.ptr = str.c_str();;
                            break;
                        }

                        case Serializable::Type::ARRAY:
                        {
                            const Serializable::Array& array = source.getArray();
                            const size_t size = array.size();

                            auto& o_zone = o.zone;
                            auto& o_array = o.via.array;
                            o_array.size = (uint32_t)size;
                            o_array.ptr = static_cast<msgpack::object*>(
                                o.zone.allocate_align(sizeof(msgpack::object) * size));

                            o.type = type::ARRAY;

                            for (size_t i = 0; i < size; ++i) {
                                o_array.ptr[i] = msgpack::object(array[i], o_zone);
                            }
                        }
                        break;

                        case Serializable::Type::MAP:
                        {
                            const Serializable::Map& map = source.getMap();
                            const size_t size = map.size();

                            auto& o_zone = o.zone;
                            auto& o_map = o.via.map;
                            o_map.size = (uint32_t)size;
                            o_map.ptr = static_cast<msgpack::object_kv*>(
                                o.zone.allocate_align(
                                    sizeof(msgpack::object_kv) * size));

                            o.type = type::MAP;
                            size_t i = 0;

                            for (const auto& kv : map) {
                                o_map.ptr[i++] = msgpack::object_kv {
                                    msgpack::object(kv.first, o_zone),
                                    msgpack::object(kv.second, o_zone)
                                };
                            }
                        }
                        break;

                        default:
                            ALERT_MSG(false, "Unknown Serializable::Type(%d)", source.getType());
                            break;
                    }
                }
            };

        } // namespace adaptor
    } // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

std::ostream& MsgpackSerializer::pack(const Serializable& source, std::ostream& outStream) const
{
    msgpack::pack(outStream, source);
    return outStream;
}

Serializable MsgpackSerializer::unpack(std::istream& inStream) const
{
    // copies all data into buffer
    std::vector<char> buffer(std::istreambuf_iterator<char>(inStream), {});

    const char* data = buffer.data();
    const size_t size = buffer.size();
    
    return msgpack::unpack(data, size).get().as<Serializable>();
}
