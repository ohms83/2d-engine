//
//  TextTypingAnimation.cpp
//  MiniRPG
//
//  Created by nuttachai on 1/5/20.
//

#include "TypingAnimation.hpp"

const int TypingAnimation::DEFAULT_SPEED = 30;

TypingAnimation::TypingAnimation()
{
    setTypingSpeed(DEFAULT_SPEED);
}

TypingAnimation::~TypingAnimation()
{
    CCLOG("Deallocating TypingAnimation");
}

TypingAnimation* TypingAnimation::create(const std::string& text, int speed)
{
    TypingAnimation* result = new TypingAnimation();
    result->setText(text);
    result->setTypingSpeed(speed);
    result->autorelease();
    return result;
}

TypingAnimation* TypingAnimation::createWithCallback(const std::string& text, int speed, TypingCallback callback)
{
    TypingAnimation* result = new TypingAnimation();
    result->setText(text);
    result->setTypingSpeed(speed);
    result->setCallback(callback);
    result->autorelease();
    return result;
}

cocos2d::FiniteTimeAction* TypingAnimation::clone() const
{
    TypingAnimation* cloneObj = new TypingAnimation();
    cloneObj->_targetLabel = _targetLabel;
    cloneObj->_targetTextUtf32 = _targetTextUtf32;
    cloneObj->_maxCharacterIndex = _maxCharacterIndex;
    cloneObj->_characterIndex = _characterIndex;
    cloneObj->_callback = _callback;
    cloneObj->_isReverse = _isReverse;
    cloneObj->setTypingSpeed(_speed);
    return cloneObj;
}

cocos2d::FiniteTimeAction* TypingAnimation::reverse() const
{
    std::string copyText;
    cocos2d::StringUtils::UTF32ToUTF8(_targetTextUtf32, copyText);
    
    TypingAnimation* cloneObj = createWithCallback(copyText, _speed, _callback);
    cloneObj->_isReverse = true;
    return cloneObj;
}

void TypingAnimation::startWithTarget(cocos2d::Node* target)
{
    cocos2d::Action::startWithTarget(target);
    
    _targetLabel = dynamic_cast<cocos2d::Label*>(target);
    CCASSERT(_targetLabel, "Only cocos2d::Label or its sub classes can run TypingAnimation.");
    
    if(_targetLabel)
    {
        _targetLabel->setString(std::move(getStartString()));
        _characterIndex = getStartCharIndex();
    }
}

bool TypingAnimation::isDone() const
{
    if(!_targetLabel)
    {
        return true;
    }
    return (_isReverse ? _characterIndex == 0 : _characterIndex >= _maxCharacterIndex);
}

void TypingAnimation::update(float time)
{
    if(!_targetLabel)
    {
        return;
    }
    
    size_t charIndex = getCurrentCharIndex(time);
    
    if(charIndex != _characterIndex)
    {
        _characterIndex = std::min(charIndex, _maxCharacterIndex);
        
        cocos2d::StringUtils::UTF32ToUTF8(_targetTextUtf32.substr(0, _characterIndex), _animateText);
        
        if(_callback)
        {
            _callback(_animateText);
        }
        
        _targetLabel->setString(_animateText);
    }
}

void TypingAnimation::setTypingSpeed(int speed)
{
    if(speed > 0)
    {
        _speed = speed;
        computeDuration();
    }
}

void TypingAnimation::setText(const std::string& text)
{
    _animateText.clear();
    _animateText.reserve(text.size() + 1);
    cocos2d::StringUtils::UTF8ToUTF32(text, _targetTextUtf32);
    _maxCharacterIndex = _targetTextUtf32.size();
    computeDuration();
}

void TypingAnimation::computeDuration()
{
    if(_speed > 0)
    {
        float interval = 1.0f / _speed;
        _duration = std::ceil(interval * _maxCharacterIndex);
        _normalizedInterval = (_duration > 0 ? interval / _duration : 1);
    }
    else
    {
        _duration = 0;
        _normalizedInterval = 1;
    }
}

size_t TypingAnimation::getStartCharIndex() const
{
    return (_isReverse ? _maxCharacterIndex : 0);
}

size_t TypingAnimation::getCurrentCharIndex(float time) const
{
    if(time >= 1.0f)
    {
        return (_isReverse ? 0 : _maxCharacterIndex);
    }
    if(_isReverse)
    {
        time = 1.0f - time;
    }
    
    return (size_t)(time / _normalizedInterval);
}

std::string TypingAnimation::getStartString() const
{
    std::string result;
    if(_isReverse)
    {
        cocos2d::StringUtils::UTF32ToUTF8(_targetTextUtf32, result);
    }
    return result;
}
