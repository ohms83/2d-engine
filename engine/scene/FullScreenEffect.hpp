#pragma once

#include "cocos2d.h"

/// A specialized version of @c cocos2d::FadeTo which enables @c _cascadeOpacityEnabled flag before begin fading
class CascadeFade : public cocos2d::FadeTo
{
public:
    /** 
     * Creates an action with duration and opacity.
     * @param duration Duration time, in seconds.
     * @param opacity A certain opacity, the range is from 0 to 255.
     * @return An autoreleased @c CascadeFade object.
     */
    static CascadeFade* create(float duration, uint8_t opacity);
    virtual void startWithTarget(cocos2d::Node *target) override;
};

/// A specialized version of @c cocos2d::FadeIn which enables @c _cascadeOpacityEnabled flag before begin fading
class CascadeFadeIn : public cocos2d::FadeIn
{
public:
    /** 
     * Creates an action with duration and opacity.
     * @param duration Duration time, in seconds.
     * @return An autoreleased @c CascadeFadeIn object.
     */
    static CascadeFadeIn* create(float duration);
    virtual void startWithTarget(cocos2d::Node *target) override;
};

/// A specialized version of @c cocos2d::FadeOut which enables @c _cascadeOpacityEnabled flag before begin fading
class CascadeFadeOut : public cocos2d::FadeOut
{
public:
    /** 
     * Creates an action with duration and opacity.
     * @param duration Duration time, in seconds.
     * @return An autoreleased @c CascadeFadeIn object.
     */
    static CascadeFadeOut* create(float duration);
    virtual void startWithTarget(cocos2d::Node *target) override;
};