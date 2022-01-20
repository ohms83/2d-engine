#include "EventHandler.hpp"

#include "engine/debug/DebugMacros.h"
#include "engine/util/TypeCheck.h"

USING_NS_CC;
using namespace std;


EventHandler::EventHandler()
{
    m_eventDispatcher = Director::getInstance()->getEventDispatcher();
}

EventHandler::~EventHandler()
{
    m_eventDispatcher = nullptr;
    CC_SAFE_RELEASE_NULL(m_eventTarget);
    CC_SAFE_RELEASE_NULL(m_listener);
}

void EventHandler::attach(cocos2d::Node* target)
{
    if (target == m_eventTarget)
    {
        if (target) {
            LOG_WARNING("Event handler already attached to target NAME=%s TAG=%d", target->getName().c_str(), target->getTag());
        }
        return;
    }

    m_eventDispatcher->removeEventListener(m_listener);
    if (target)
    {
        m_listener = onAttached(target);
        if (m_listener)
        {
            m_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, target);
            m_listener->retain();
        }
    }
    else if (m_eventTarget)
    {
        onDettached(m_eventTarget);
        CC_SAFE_RELEASE_NULL(m_listener);
    }

    CC_SAFE_RELEASE_NULL(m_eventTarget);
    m_eventTarget = target;
    CC_SAFE_RETAIN(m_eventTarget);
}

bool EventHandler::isAttached() const
{
    return m_eventTarget != nullptr;
}