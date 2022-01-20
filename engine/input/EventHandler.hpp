#pragma once

#include "cocos2d.h"

class EventHandler
{
public:
    EventHandler();
    ~EventHandler();

    /// Attach this event handler to the target node. Pass @c nullptr to detach.
    void attach(cocos2d::Node* target);
    bool isAttached() const;

protected:
    /**
     * Callback that will be triggered when handler is about to be detached to the new target.
     * @return If a pointer of @c cocos2d::EventListener is returned, it will be attached to @c newTarget
     */
    virtual cocos2d::EventListener* onAttached(cocos2d::Node* newTarget) { return nullptr; }
    /// Callback that will be triggered when handler is being detached from the current target.
    virtual void onDettached(cocos2d::Node* currentTarget) {}

protected:
    cocos2d::Node* m_eventTarget = nullptr;
    cocos2d::EventListener* m_listener = nullptr;
    cocos2d::EventDispatcher* m_eventDispatcher = nullptr;
};