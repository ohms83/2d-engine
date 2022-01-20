#pragma once

#include "cocos2d.h"
#include "engine/animation/AnimationManager.hpp"
#include "engine/util/TypeCheck.h"

namespace base
{
    /**
     *  Avatar is the base class of all objects that have presentation; eg., players, items, etc.
     *  You can think about it as Sprite that can animate.
     */
    class Avatar : public cocos2d::Sprite
    {
    public:
        Avatar();
        virtual ~Avatar();

        animation::AnimationManager& getAnimationManager()
        {
            return m_animManager;
        }

        const animation::AnimationManager& getAnimationManager() const
        {
            return m_animManager;
        }

        /// Load animation from the given list of animation files and register them to this avatar.
        void loadAnimationList(const std::vector<std::string>& fileList);

    protected:
        animation::AnimationManager m_animManager;
    };
}
