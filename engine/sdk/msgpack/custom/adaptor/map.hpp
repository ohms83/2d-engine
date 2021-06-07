//
//  MessagePack custom map adaptor
//  Created by nuttachai on 22/11/19.
//  Reference: https://github.com/msgpack/msgpack-c/issues/523
//

#ifndef MSGPACK_CUSTOM_MAP_HPP
#define MSGPACK_CUSTOM_MAP_HPP

#include "msgpack/adaptor/map_decl.hpp"
#include "msgpack/v1/adaptor/map.hpp"

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace adaptor {
    template<>
    struct convert<msgpack::object_map>
    {
        const msgpack::object& operator()(const msgpack::object& o, msgpack::object_map& map) const
        {
            if (o.type != msgpack::type::MAP)
                throw msgpack::type_error();
            map = o.via.map;
            return o;
        }
    };
} // namespace adaptor

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

}  // namespace msgpack


#endif // MSGPACK_CUSTOM_MAP_HPP
