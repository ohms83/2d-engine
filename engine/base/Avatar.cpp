#include "Avatar.hpp"

USING_NS_CC;

using namespace std;
using namespace base;

Avatar::Avatar()
{
}

Avatar::~Avatar()
{
    CC_SAFE_RELEASE(m_avatarSprite);
}

bool Avatar::initWithFile(const std::string& fileName)
{
    m_avatarSprite = Sprite::create(fileName);
    CHECK_IF_NULL_RETURN_MSG(m_avatarSprite, false, "Error initializing avatar. REASON=Invalid file path. FILE=%s", fileName.c_str());
    
    atachAvatarSprite();
    
    return init();
}

bool Avatar::initWithSpriteFrame(const std::string& spriteFrameName)
{
    static SpriteFrameCache* spriteFrameCache = SpriteFrameCache::getInstance();
    
    SpriteFrame* spriteFrame = spriteFrameCache->getSpriteFrameByName(spriteFrameName);
    CHECK_IF_NULL_RETURN_MSG(spriteFrame, false, "Error initializing avatar. REASON=Invalid sprite frame. FILE=%s", spriteFrameName.c_str());
    
    m_avatarSprite = Sprite::createWithSpriteFrame(spriteFrame);
    CHECK_IF_NULL_RETURN_MSG(m_avatarSprite, false, "Error initializing avatar. REASON=Invalid sprite frame. FILE=%s", spriteFrameName.c_str());
    
    atachAvatarSprite();
    
    return init();
}

void Avatar::atachAvatarSprite()
{
    CC_SAFE_RETAIN(m_avatarSprite);
    
    const cocos2d::Size& size = m_avatarSprite->getContentSize();
    setContentSize(size);
    
    const Vec2& anchorPoint = m_avatarSprite->getAnchorPoint();
    setAnchorPoint(anchorPoint);
    
    addChild(m_avatarSprite);
    
    m_animManager.attach(m_avatarSprite);
    
    if(!_contentSize.equals(cocos2d::Size::ZERO)) {
        m_avatarSprite->setContentSize(_contentSize);
    }
    else {
        GameObject::setContentSize(m_avatarSprite->getContentSize());
    }
    
}

void Avatar::setContentSize(const cocos2d::Size& size)
{
    GameObject::setContentSize(size);
    if (m_avatarSprite) {
        m_avatarSprite->setContentSize(size);
    }
}

void Avatar::setAnchorPoint(const Vec2& anchorPoint)
{
    GameObject::setAnchorPoint(anchorPoint);
    
    if (m_avatarSprite)
    {
        m_avatarSprite->setAnchorPoint(anchorPoint);
        // Move the sprite to avatar's origin.
        m_avatarSprite->setPosition(_anchorPointInPoints);
    }
}
