#include "Behaviour.hpp"
#include "GameObject.hpp"

#include "engine/debug/Log.hpp"
#include "engine/util/TypeCheck.h"

USING_NS_CC;
using namespace std;
using namespace base;

Behaviour::Behaviour()
{
}

Behaviour::~Behaviour()
{
    LOG_ASSERT(m_gameObject == nullptr, "Behaviour MUST NOT be deleted before detaching it from gameObject; it will cause heap corruption.");
}

void Behaviour::enable(bool flag)
{
    if(m_isEnable != flag)
    {
        m_isEnable = flag;
        
        if(m_isEnable && scheduleUpdate()) {
            onEnabled();
        }
        if(!m_isEnable && unscheduleUpdate()) {
            onDisbled();
        }
    }
}

void Behaviour::setPriority(unsigned int priority)
{
    if(priority != m_priority)
    {
        m_priority = priority;

        if(isEnable())
        {
            // Reshcedule update with the new priority.
            unscheduleUpdate();
            scheduleUpdate();
        }
    }
}

bool Behaviour::scheduleUpdate()
{
    CHECK_IF_NULL_RETURN(m_gameObject, false);
    m_gameObject->getScheduler()->scheduleUpdate(this, (int)m_priority, false);
    return true;
}

bool Behaviour::unscheduleUpdate()
{
    CHECK_IF_NULL_RETURN(m_gameObject, false);
    m_gameObject->getScheduler()->unscheduleUpdate(this);
    return true;
}

void Behaviour::attachTo(GameObject* gameObject)
{
    m_gameObject = gameObject;
    if (isEnable()) {
        scheduleUpdate();
    }
    
    onAttached(gameObject);
}

void Behaviour::removeFrom(GameObject* gameObject)
{
    if (isEnable()) {
        unscheduleUpdate();
    }
    
    onRemoved(gameObject);
    m_gameObject = nullptr;
}
