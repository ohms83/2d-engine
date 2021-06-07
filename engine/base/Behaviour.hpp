#pragma once

#include "cocos2d.h"

#include <string>

namespace base
{
    class GameObject;

    /**
     *  Behaviour represents game logics that can be attached/removed at run-time.
     *  @remark
     *  Unlike @c cocos2d::Action , @c Behaviour is more suit for long running game logics (AI, physics, etc.),
     *  or detachable event listeners (UI or controller logics).
     */
    class Behaviour : public cocos2d::Ref
    {
        friend class GameObject;
    public:
        Behaviour();
        virtual ~Behaviour();

        /**
         *  Enable/disable the current behaviour.
         *  @param flag @c true to enable and @c false to disable.
         *
         *  Upon enabled, @c update function will be scheduled with the current priority and will be started
         *  being called every frames. Interally, @c onEnabled function will be triggered before the first update.
         *  When disabled, @c update funtion will be removed from the scheduler and stop receiving any call
         *  and @c onDisbled function will be triggered.
         *  
         *  @note Nothing will happen if the enable flag is the same as the current one.
         */
        void enable(bool flag);

        bool isEnable() const
        {
            return m_isEnable;
        }

        /**
         *  Change behaviour's priority.
         *  @param priority The lower priority, the earlier it will be updated. If the behaviour is already enabled,
         *                  it's @c update function will be reschedule with the given priority.
         */
        void setPriority(unsigned int priority);
        /// Get current behaviour's priority. Priority can only be changed by calling @c enable function.
        unsigned int getPriority() const
        {
            return m_priority;
        }
        
        /// Update is called once per frame.
        virtual void update(float dt) {}
        
    protected:
        /**
         *  @c onAttached is called when the behaviour is attached.
         *  @param gameObject Pointer to the target @c GameObject that this behaviour being attched to.
         *                    It's guaranteed to be non-null.
         */
        virtual void onAttached(GameObject* gameOjbect) {}
        /**
         *  @c onRemoved is called when the behaviour is removed.
         *  @param gameObject Pointer to the target @c GameObject that this behaviour being removed from.
         *                    It's guaranteed to be non-null.
         */
        virtual void onRemoved(GameObject* gameOjbect) {}

        /// @c onEnabled is called when behaviour becomes active.
        virtual void onEnabled() {}
        /// @c onDisbled is called when behaviour becomes inactive.
        virtual void onDisbled() {}
        
    private:
        void attachTo(GameObject* gameObject);
        void removeFrom(GameObject* gameObject);
        
        bool scheduleUpdate();
        bool unscheduleUpdate();

    protected:
        /// Target @c GameObject that this behaviour is being attached to.
        GameObject* m_gameObject;
        bool m_isEnable = true;
        unsigned int m_priority = 0;
    };
}
