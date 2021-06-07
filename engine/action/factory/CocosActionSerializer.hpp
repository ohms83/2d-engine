//
//  CocosActionSerializer.hpp
//  MiniRPG
//
//  A collections of cocos2d::Action serializers.
//
//  Created by nuttachai on 23/5/20.
//

#ifndef CocosActionSerializer_hpp
#define CocosActionSerializer_hpp

#include "cocos2d.h"

#include "engine/serialize/Serialize.hpp"

#include <fstream>

namespace action
{
    /// Create cocos2d::Animate action from the specified @p filePath using the given @p serializer from specified Factory class
    template<class Factory, class ActionType>
    ActionType* createActionFromFile(const data::Serializer& serializer, const std::string& filePath)
    {
        std::ifstream ifs(filePath);
        if(!ifs.good())
        {
            CCLOGWARN("%s cannot be found!", filePath.c_str());
            return nullptr;
        }
        data::Serializable obj = std::move(serializer.unpack(ifs));
        return dynamic_cast<ActionType*>(Factory::create(obj));
    }
}
class DelayTimeSerializer final
{
public:
    /// Create cocos2d::DelayTime action from the @p serializeObj
    static cocos2d::Action* create(const data::Serializable& serializedObj);
};

class AnimateSerializer final
{
public:
    /// Create cocos2d::Animate action from the @p serializeObj
    static cocos2d::Action* create(const data::Serializable& serializedObj);
};

#endif /* CocosActionSerializer_hpp */
