#pragma once

#include "cocos2d.h"

#include "engine/pattern/Singleton.hpp"
#include "engine/pattern/Factory.hpp"

#include <string>
#include <functional>

class SceneFactory : public pattern::Singleton<SceneFactory>, public pattern::FactoryString<cocos2d::Scene>
{
public:
    SceneFactory();
};

#define CREATE_SCENE(_discreetClass) SceneFactory::getInstance().create(#_discreetClass)