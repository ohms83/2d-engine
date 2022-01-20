#pragma once

#include "cocos2d.h"
#include "EventHandler.hpp"

class ControllerEventHandler : public EventHandler
{
public:
    ControllerEventHandler();
    ~ControllerEventHandler();

    bool isControllerEnabled() const;

protected:
    cocos2d::EventListener* onAttached(cocos2d::Node* target) override;

private:
    virtual void onConnected(cocos2d::Controller* controller, cocos2d::Event* event) {}
	virtual void onDisconnected(cocos2d::Controller* controller, cocos2d::Event* event) {}
    
    virtual void onKeyDown(cocos2d::Controller* controller, int key, cocos2d::Event* event) {}
    virtual void onKeyUp(cocos2d::Controller* controller, int key, cocos2d::Event* event) {}
	virtual void onKeyRepeat(cocos2d::Controller* controller, int key, cocos2d::Event* event) {}
    
	virtual void onAxisEvent(cocos2d::Controller* controller, int key, cocos2d::Event* event) {}

private:
};