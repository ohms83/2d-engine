//
//  AnimationManager.cpp
//  MiniRPG
//
//  Created by nuttachai on 17/5/20.
//

#include "AnimationManager.hpp"
#include "AnimationUtil.hpp"
#include "util/TypeCheck.h"
#include "util/StringUtil.hpp"

#include <filesystem>

using namespace animation;
using namespace debug;

USING_NS_CC;
using namespace std;

AnimationManager::AnimationManager()
{
    
}

AnimationManager::~AnimationManager()
{
    CC_SAFE_RELEASE(m_target);
    CC_SAFE_RELEASE(m_runningAction);
}

void AnimationManager::attach(Sprite* target)
{
    CC_SAFE_RELEASE(m_target);
    m_target = target;
    CC_SAFE_RETAIN(m_target);
}

void AnimationManager::load(const string& file)
{
    Animation* anim = animationUtil::createAnimation(file);

    stringstream ss;
    ss << std::filesystem::path(file).filename().replace_extension("");
    // std::filesystem::path::filename() returns file name enclosed by double quotes. They need to be removed.
    string fileName = util::strutil::trim(ss.str(), "\f\n\r\t\v\"");
    LOG_DEBUG("%s", fileName.c_str());

    m_animList.insert(fileName, anim);
    
    int baseTag = cocos2d::random();
    int index = 0;
    
    for (const auto& keyValue : m_animList)
    {
        const string& animName = keyValue.first;
        m_animTags.emplace(animName, baseTag + (index++));
    }
}

void AnimationManager::loadList(const set<string>& fileList)
{
    for (const auto& file : fileList)
    {
        load(file);
    }
}

void AnimationManager::play(const string& animName)
{
    CHECK_IF_NULL_MSG(m_target, "Error. There's no attached Sprite target.");
    
    if (isPlaying(animName))
    {
        return;
    }
    
    auto itr = m_animList.find(animName);
    if (m_animList.end() == itr)
    {
        LOG_WARNING("Animation not found ANIMATION=%s", animName.c_str());
        return;
    }
    
    if (m_runningAction)
    {
        m_target->stopAction(m_runningAction);
        m_runningAction->release();
    }
    
    Animate* animateAction = Animate::create(itr->second);
    CallFunc* animEndCallback = CallFunc::create([this] {
        onAnimEnd();
    });
    
    m_runningAction = Sequence::create(animateAction, animEndCallback, NULL);
    
    if (m_runningAction)
    {
        m_runningAction->setTag(m_animTags.at(animName));
        m_runningAction->retain();
        
        m_target->runAction(m_runningAction);
        m_runningAnimationName = animName;
    }
}

void AnimationManager::stop()
{
    CHECK_IF_NULL_MSG(m_target, "Error. There's no attached Sprite target.");
    
    if (m_runningAction)
    {
        m_target->stopAction(m_runningAction);
        CC_SAFE_RELEASE_NULL(m_runningAction);
    }
    
    m_runningAnimationName.clear();
}

bool AnimationManager::isPlaying(const string& animName) const
{
    if (!isPlaying())
    {
        return false;
    }
    
    CHECK_IF_NULL_RETURN_MSG(m_target, false, "Error. There's no attached Sprite target.");
    
    if (m_animList.find(animName) == m_animList.end())
    {
        LOG_WARNING(" Animation not found ANIMATION=%s", animName.c_str());
        return false;
    }
    
    const int tag = m_animTags.at(animName);
    if (tag != m_runningAction->getTag())
    {
        return false;
    }
    
    Vector<Action*> actions = m_target->getAllActionsByTag(tag);
    return actions.getIndex(m_runningAction) != -1;
}

bool AnimationManager::isPlaying() const
{
    return m_runningAction && !m_runningAction->isDone();
}

void AnimationManager::onAnimEnd()
{
    if (m_animEndCallback) {
        m_animEndCallback(m_runningAnimationName);
    }
    
    CC_SAFE_RELEASE_NULL(m_runningAction);
    m_runningAnimationName.clear();
}

void AnimationManager::registerAnimEndCallback(function<void(const string&)> callback)
{
    m_animEndCallback = callback;
}

void AnimationManager::unregisterAnimEndCallback()
{
    m_animEndCallback = nullptr;
}
