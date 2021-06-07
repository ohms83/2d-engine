//
//  Serialize.hpp
//  MiniRPG
//
//  Created by nuttachai on 30/10/19.
//

#ifndef JsonSerializer_hpp
#define JsonSerializer_hpp

#include "Serialize.hpp"
#include <sstream>

namespace serialize
{
	class JsonSerializer final : public Serializer
	{
	public:
        std::ostream& pack(const Serializable& source, std::ostream& outStream) const override;
        Serializable unpack(std::istream& inStream) const override;

		std::string toString(const Serializable& source) const;
		Serializable fromString(const std::string& inString) const;
	};
}

#endif
