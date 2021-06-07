//
//  ActionFactory.cpp
//  MiniRPG
//
//  Created by nuttachai on 23/5/20.
//

#include "ActionFactory.hpp"
#include "CocosActionSerializer.hpp"
#include "engine/debug/DebugMacros.h"
#include "engine/util/StringExt.hpp"
#include "engine/util/Common.hpp"

USING_NS_CC;
using namespace std; 

static ActionFactory* s_singletonInstance = nullptr;

ActionFactory::ActionFactory()
{
    initialize();
}

ActionFactory& ActionFactory::getInstance()
{
    if (!s_singletonInstance)
    {
        s_singletonInstance = new ActionFactory();
    }
    return *s_singletonInstance;
}

bool ActionFactory::initialize()
{
    registerSerializer("delaytime", DelayTimeSerializer::create);
    return true;
}

cocos2d::Action* ActionFactory::parse(std::string data, const data::Serializer& serializer) const
{
    stringstream ss(data);
    return parse(ss, serializer);
}

Action* ActionFactory::parse(std::istream& instream, const data::Serializer& serializer) const
{
    return unpack(serializer.unpack(instream));
}

cocos2d::Action* ActionFactory::unpack(const data::Serializable& serializedObj) const
{
    const data::Serializable::Type objType = serializedObj.getType();
    
    switch (objType)
    {
        case data::Serializable::Type::ARRAY:
            return unpackArray(serializedObj.getArray());
        
        case data::Serializable::Type::MAP:
            return unpackObj(serializedObj.getMap());
            
        default:
            CCLOGWARN("ERROR: Unsupport serializable object (%s). The root object can only be either "
                      "data::Serializable::Type::ARRAY or data::Serializable::Type::MAP.",
                      data::Serializable::getTypeString(objType).c_str());
            break;
    }
    return nullptr;
}

Action* ActionFactory::unpackArray(const data::Serializable::Array& serializedObj) const
{
    cocos2d::Vector<FiniteTimeAction*> actionList;
    
    for (const auto& elem : serializedObj )
    {
        const data::Serializable::Type objType = elem.getType();
        if (objType != data::Serializable::Type::MAP)
        {
            CCLOGWARN("ERROR: Unsupport serializable object (%s). The items inside array can only be data::Serializable::Type::MAP.",
                      data::Serializable::getTypeString(objType).c_str());
            continue;
        }
        
        FiniteTimeAction* action = dynamic_cast<FiniteTimeAction*>(unpackObj(elem));
        if (!action)
        {
            CCLOGWARN("ERROR: Unsupport action (%s). Only cocos2d::FiniteTimeAction can be sequential.", util::common::getClassName(action).c_str());
            continue;
        }
        
        actionList.pushBack(action);
    }
    
    if (!actionList.empty())
    {
        Sequence* sequence = Sequence::create(actionList);
        return sequence;
    }
    else
    {
        return nullptr;
    }
}

cocos2d::Action* ActionFactory::unpackObj(const data::Serializable::Map& serializedObj) const
{
    const std::string& actionType = serializedObj.at("action").getString();
    if (actionType.empty())
    {
        return nullptr;
    }
    
    return create(actionType, serializedObj);
}

Action* ActionFactory::create(const string& typeName, const data::Serializable& serializedObj) const
{
    string typeNameLower = util::strExt::tolowerCopy(typeName);
    auto itr = m_serializers.find(typeNameLower);

    if (itr != m_serializers.end())
    {
        SerilazerFunction create = itr->second;
        return create(serializedObj);
    }
    
    CCLOGWARN("ERROR: Cannot find SerilazerFunction of type \'%s\'", typeName.c_str());
    return nullptr;
}

void ActionFactory::registerSerializer(const std::string& typeName, SerilazerFunction serializer)
{
    string typeNameLower = util::strExt::tolowerCopy(typeName);
    if (!serializer)
    {
        NOTICE_WARNING( "NULL function cannot be registered!" );
        return;
    }
    
    m_serializers[typeNameLower] = serializer;
}
