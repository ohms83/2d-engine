//
//  ActionFactory.hpp
//  MiniRPG
//
//  Created by nuttachai on 23/5/20.
//

#ifndef ActionFactory_hpp
#define ActionFactory_hpp

#include "cocos2d.h"

#include "engine/serialize/Serialize.hpp"

#include <functional>
#include <string>
#include <map>

/** Factory class that help creating actions from scripts.
 *
 */
class ActionFactory final
{
public:
    typedef std::function<cocos2d::Action*(const data::Serializable& serializedObj)> SerilazerFunction;
    
    ActionFactory();
    
    /** Singleton accessor.
     *  @note You don't need to use singleton. ActionFactory can be created just like the normal classes.
     */
    static ActionFactory& getInstance();
    
    //! Register factory with the known action types.
    bool initialize();
    
    //! Using the specified serializer to parse the given data and crate an Action object.
    cocos2d::Action* parse(std::string data, const data::Serializer& serializer) const;
    
    //! Using the specified serializer to parse the given input stream and crate an Action object.
    cocos2d::Action* parse(std::istream& instream, const data::Serializer& serializer) const;
    
    /** Try unpacking the given serializedObj using registered serializers.
     *  @return An object of known Action subclass; or nullptr, if there's no registered serializers can unpack it.
     */
    cocos2d::Action* unpack(const data::Serializable& serializedObj) const;
    
    /** Create an action from the registered SerilazerFunction with typeName.
     *  @return An object of Action subclass that was registered with typeName; or nullptr, if typeName cannot be found.
     */
    cocos2d::Action* create(const std::string& typeName, const data::Serializable& serializedObj) const;
    
    void registerSerializer(const std::string& typeName, SerilazerFunction serializer);
    
private:
    cocos2d::Action* unpackArray(const data::Serializable::Array& serializedObj) const;
    cocos2d::Action* unpackObj(const data::Serializable::Map& serializedObj) const;
    
    std::map<std::string, SerilazerFunction> m_serializers;
};

#endif /* ActionFactory_hpp */
