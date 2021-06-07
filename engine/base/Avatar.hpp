#pragma once

#include "cocos2d.h"
#include "GameObject.hpp"
#include "engine/animation/AnimationManager.hpp"
#include "engine/util/TypeCheck.h"

namespace base
{
    /**
     *  Avatar is the base class of all objects that have presentation; eg., players, items, etc.
     *  You can think about it as Sprite that can animate.
     */
    class Avatar : public GameObject
    {
    public:
        Avatar();
        virtual ~Avatar();

        /**
         *  Create an auto-release object of any Avatar sub-types from file.
         *  This is provided so that child classes don't need to implment their own variations if this function.
         */
        template<typename _Class>
        static _Class* create(const std::string& fileName)
        {
            _Class* avatar = new (std::nothrow) _Class();
            avatar->autorelease();
            
            CHECK_IF_RETURN_MSG(!avatar->initWithFile(fileName), nullptr, "Error creating avatar. REASON=Invalid file path. FILE=%s", fileName.c_str());
            return avatar;
        }

        /**
         *  Create an auto-release object of any Avatar sub-types from the specified sprite frame.
         *  This is provided so that child classes don't need to implment their own variations if this function.
         */
        template<typename _Class>
        static _Class* createFromSpriteFrame(const std::string& spriteFrameName)
        {
            static cocos2d::SpriteFrameCache* spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
            
            cocos2d::SpriteFrame* spriteFrame = spriteFrameCache->getSpriteFrameByName(spriteFrameName);
            CHECK_IF_NULL_RETURN_MSG(spriteFrame, nullptr, "Error creating avatar. REASON=Invalid sprite frame. FILE=%s", spriteFrameName.c_str());
            
            _Class* avatar = new (std::nothrow) _Class();
            avatar->autorelease();
            
            CHECK_IF_RETURN_MSG(!avatar->initWithSpriteFrame(spriteFrameName), nullptr, "Error creating avatar. REASON=Invalid sprite frame. FILE=%s", spriteFrameName.c_str());
            return avatar;
        }
        
        bool initWithFile(const std::string& fileName);
        bool initWithSpriteFrame(const std::string& spriteFrameName);

        animation::AnimationManager& getAnimationManager()
        {
            return m_animManager;
        }

        const animation::AnimationManager& getAnimationManager() const
        {
            return m_animManager;
        }

        virtual void setContentSize(const cocos2d::Size& size) override;
        virtual void setAnchorPoint(const cocos2d::Vec2& anchorPoint) override;
        
    private:
        void atachAvatarSprite();

    protected:
        cocos2d::Sprite* m_avatarSprite = nullptr;
        animation::AnimationManager m_animManager;
    };
}
