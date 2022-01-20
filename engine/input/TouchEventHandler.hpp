#pragma once

#include "cocos2d.h"
#include "EventHandler.hpp"

class TouchEventHandler : public EventHandler
{
public:
    TouchEventHandler();
    ~TouchEventHandler();

    bool isTouchEnabled() const;

protected:
    cocos2d::EventListener* onAttached(cocos2d::Node* target) override;

private:
    /**
     * @return @c true , to consume the event and stop propagation; otherwise, @c false
     */
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
    {
        return false;
    }
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {}
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {}
    virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event) {}
};