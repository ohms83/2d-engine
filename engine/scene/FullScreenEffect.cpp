#include "FullScreenEffect.hpp"

USING_NS_CC;
using namespace std;

CascadeFade* CascadeFade::create(float duration, uint8_t opacity)
{
    CascadeFade* action = new (std::nothrow) CascadeFade();
    if (action && action->initWithDuration(duration, opacity))
    {
        action->autorelease();
        return action;
    }

    CC_SAFE_DELETE(action);
    return nullptr;
}

void CascadeFade::startWithTarget(Node *target)
{
    if (target) {
        target->setCascadeOpacityEnabled(true);
    }
    FadeTo::startWithTarget(target);
}

CascadeFadeIn* CascadeFadeIn::create(float duration)
{
    CascadeFadeIn* action = new (std::nothrow) CascadeFadeIn();
    if (action && action->initWithDuration(duration, 0xFF))
    {
        action->autorelease();
        return action;
    }

    CC_SAFE_DELETE(action);
    return nullptr;
}

void CascadeFadeIn::startWithTarget(Node *target)
{
    if (target) {
        target->setCascadeOpacityEnabled(true);
    }
    FadeIn::startWithTarget(target);
}

CascadeFadeOut* CascadeFadeOut::create(float duration)
{
    CascadeFadeOut* action = new (std::nothrow) CascadeFadeOut();
    if (action && action->initWithDuration(duration, 0))
    {
        action->autorelease();
        return action;
    }

    CC_SAFE_DELETE(action);
    return nullptr;
}

void CascadeFadeOut::startWithTarget(Node *target)
{
    if (target) {
        target->setCascadeOpacityEnabled(true);
    }
    FadeOut::startWithTarget(target);
}