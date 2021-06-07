#pragma once

#include <string>

#include "cocos2d.h"

#include "Behaviour.hpp"

#include "engine/util/CocosUtil.hpp"
#include "engine/util/Common.hpp"

/// Create and attach a behaviour of @c _Type to the @c _gameObject .
#define ATTACH_BEHAVIOUR(_gameObject, _Type) _gameObject->attachBehaviour(#_Type, util::cocosutil::create<_Type>())
/// Get a behaviour of type @c _Type from @c _gameObject .
#define GET_BEHAVIOUR(_gameObject, _Type) dynamic_cast<_Type*>(_gameObject->getBehaviour(#_Type))
/// Remove a behaviour of type @c _Type from @c _gameObject .
#define REMOVE_BEHAVIOUR(_gameObject, _Type) _gameObject->removeBehaviour(#_Type)

namespace base
{
    /**
     *  This is a copied concept from Unity's GameObject/Behaviour model.
     *  @discuss
     *  To promote codes reuseability, game logics can be broken down into multiple behaviours. These behaviour can be
     *  attached to or removed from the game-object at run-time; meaning that, each object in game only need to house
     *  logics that relevance to them (no more uber classes or heirachy hell). This will result in lighter classes that
     *  can be easily maintained and reuse.
     */
    class GameObject : public cocos2d::Node
    {
    public:
        virtual ~GameObject();
        
        /// Attach @c behaviour to current game-objct and register it with @c name
        void attachBehaviour(const std::string& name, Behaviour* behaviour);
        
        /// Helper function that attaches and registers @c behaviour using its class's name.
        template<typename _Class>
        void attachBehaviour(_Class* behaviour)
        {
            std::string className = util::common::getClassName(*behaviour);
            attachBehaviour(className, behaviour);
        }
        
        /// Remove behaviour by name
        void removeBehaviour(const std::string& name);
        /// Remove speicified behaviour
        void removeBehaviour(Behaviour* behaviour);
        void remvoveAllBehaviours();
        
        Behaviour* getBehaviour(const std::string& name);

    protected:
        cocos2d::Map<std::string, Behaviour*> m_behaviours;
    };
}
