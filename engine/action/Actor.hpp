//
//  Actor.hpp
//  MiniRPG
//
//  Created by nuttachai on 18/5/20.
//

#ifndef Actor_hpp
#define Actor_hpp

#include "cocos2d.h"

/** Actor is an interface for managing groups of actions for the node.
 *  You don't need to use this class to run actions, however, Actor comes with
 *  some convenient functions that help running and managing multiple actions
 *  a lot easier.
 */
class Actor
{
public:
    Actor();
    
    //! Attach this actor to the target node.
    void attach(cocos2d::Node* target);
    
    /** Search for a registered action by the given name and run.
     *  @return true, if success; otherwise, if there's no action found registered with the given name, false,
     */
    bool runAction(const std::string& actionName);
    
protected:
    cocos2d::Node* m_target = nullptr;
    cocos2d::Map<std::string, cocos2d::Action*> m_actions;
    
    //! A generic 
    std::function<void(cocos2d::Action*)> m_actionCallback = nullptr;
};

#endif /* Actor_hpp */
