//
//  MessagePack custom array adaptor
//  Created by nuttachai on 22/11/19.
//  Reference: https://github.com/msgpack/msgpack-c/issues/523
//

#ifndef MSGPACK_CUSTOM_ARRAY_HPP
#define MSGPACK_CUSTOM_ARRAY_HPP

#include "msgpack/v1/adaptor/cpp11/array.hpp"

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace adaptor {
    template<>
    struct convert<msgpack::object_array>
    {
        const msgpack::object& operator()(const msgpack::object& o, msgpack::object_array& array) const
        {
            if (o.type != msgpack::type::ARRAY)
                throw msgpack::type_error();
            array = o.via.array;
            return o;
        }
    };
} // namespace adaptor

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

}  // namespace msgpack

#endif // MSGPACK_CUSTOM_ARRAY_HPP
