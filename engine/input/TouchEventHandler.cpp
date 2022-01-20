#include "TouchEventHandler.hpp"

USING_NS_CC;
using namespace std;


TouchEventHandler::TouchEventHandler()
{

}

TouchEventHandler::~TouchEventHandler()
{
}

cocos2d::EventListener* TouchEventHandler::onAttached(cocos2d::Node* target)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(TouchEventHandler::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TouchEventHandler::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TouchEventHandler::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(TouchEventHandler::onTouchCancelled, this);
    return listener;
}

bool TouchEventHandler::isTouchEnabled() const
{
    return isAttached();
}