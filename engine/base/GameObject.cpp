#include "GameObject.hpp"

#include "engine/util/TypeCheck.h"
#include "engine/debug/Log.hpp"

USING_NS_CC;
using namespace std;
using namespace base;
using namespace debug;

GameObject::~GameObject()
{
    remvoveAllBehaviours();
}

void GameObject::attachBehaviour(const std::string& name, Behaviour* behaviour)
{
    CHECK_IF_NULL(behaviour);
    
    Behaviour* oldBehaviour = m_behaviours.at(name);
    if (!oldBehaviour)
    {
        m_behaviours.insert(name, behaviour);
        behaviour->attachTo(this);
    }
    else
    {
        LOG_WARNING("Behaviour is already attached. NAME=%s", name.c_str());
    }
}

void GameObject::removeBehaviour(const std::string& name)
{
    auto itr = m_behaviours.find(name);
    if (itr != m_behaviours.end())
    {
        itr->second->removeFrom(this);
        m_behaviours.erase(itr);
    }
    else
    {
        LOG_WARNING("Behaviour is already attached. NAME=%s", name.c_str());
    }
}

void GameObject::removeBehaviour(Behaviour* behaviour)
{
    CHECK_IF_NULL(behaviour);
    
    for (const auto& itr : m_behaviours)
    {
        if (itr.second == behaviour)
        {
            behaviour->removeFrom(this);
            m_behaviours.erase(itr.first);
            return;
        }
    }
    LOG_WARNING("Behaviour is not attached attached.");
}

void GameObject::remvoveAllBehaviours()
{
    for (const auto& itr : m_behaviours)
    {
        itr.second->removeFrom(this);
    }
    
    m_behaviours.clear();
}

Behaviour* GameObject::getBehaviour(const std::string& name)
{
    auto itr = m_behaviours.find(name);
    if (itr != m_behaviours.end())
    {
        return itr->second;
    }
    
    LOG_ERROR("Cannot find behaviour. NAME=%s", name.c_str());
    return nullptr;
}
