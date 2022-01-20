//
//  AnimationManager.hpp
//  MiniRPG
//
//  Created by nuttachai on 17/5/20.
//

#ifndef AnimationManager_hpp
#define AnimationManager_hpp

#include "cocos2d.h"

#include <map>
#include <set>
#include <string>
#include <functional>

namespace animation
{
    /**
     *  Manage animations for the specified target sprite.
     */
    class AnimationManager
    {
    public:
        AnimationManager();
        ~AnimationManager();
        
        /**
         *  Load animation from file.
         *  @see animation::util::createAnimationListFromJson
         */
        void load(const std::string& file);
        void loadList(const std::vector<std::string>& fileList);
        
        /// Attach this object to the specified @c target sprite.
        void attach(cocos2d::Sprite* target);
        /// Create a @c cocos2d::Animate action and run it on the attached target.
        void play(const std::string& animName);
        /// Stop currently playing animation.
        void stop();
        /// Check whether the specified animation is playing.
        bool isPlaying(const std::string& animName) const;
        /// Check whether there is any animation playing.
        bool isPlaying() const;
        
        const std::string& getRunningAnimationName() const
        {
            return m_runningAnimationName;
        }
        
        void registerAnimEndCallback(std::function<void(const std::string&)> callback);
        void unregisterAnimEndCallback();
        
    protected:
        cocos2d::Map<std::string, cocos2d::Animation*> m_animList;
        std::map<std::string, int> m_animTags;
        cocos2d::Sprite* m_target = nullptr;
        /// Running animation. There can only be one running animation at the time.
        cocos2d::Sequence* m_runningAction = nullptr;
        /// Name of currently running animation.
        std::string m_runningAnimationName;
        /// This will be called when the current animation is finished.
        std::function<void(const std::string&)> m_animEndCallback;
        
    private:
        void onAnimEnd();
    };
}

#endif /* AnimationManager_hpp */
