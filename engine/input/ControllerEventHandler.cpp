#include "ControllerEventHandler.hpp"

USING_NS_CC;
using namespace std;

ControllerEventHandler::ControllerEventHandler()
{

}

ControllerEventHandler::~ControllerEventHandler()
{
}

EventListener* ControllerEventHandler::onAttached(Node* target)
{
    auto listener = EventListenerController::create();
    
    listener->onConnected = CC_CALLBACK_2(ControllerEventHandler::onConnected, this);
    listener->onDisconnected = CC_CALLBACK_2(ControllerEventHandler::onDisconnected, this);
    
    listener->onKeyDown = CC_CALLBACK_3(ControllerEventHandler::onKeyDown, this);
    listener->onKeyUp = CC_CALLBACK_3(ControllerEventHandler::onKeyUp, this);
    listener->onKeyRepeat = CC_CALLBACK_3(ControllerEventHandler::onKeyRepeat, this);
    
    listener->onAxisEvent = CC_CALLBACK_3(ControllerEventHandler::onAxisEvent, this);

    return listener;
}

bool ControllerEventHandler::isControllerEnabled() const
{
    return isAttached();
}