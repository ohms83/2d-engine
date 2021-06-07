//
//  MsgpackSerializer.hpp
//  MiniRPG
//
//  Created by nuttachai on 16/11/19.
//

#ifndef MsgpackSerializer_hpp
#define MsgpackSerializer_hpp

#include "Serialize.hpp"
#include <sstream>

namespace serialize
{
    class MsgpackSerializer final : public Serializer
    {
    public:
        std::ostream& pack(const Serializable& source, std::ostream& outStream) const override;
        Serializable unpack(std::istream& inStream) const override;
    };
}

#endif /* MsgpackSerializer_hpp */
