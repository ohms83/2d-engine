#pragma once

#include "cocos2d.h"

#include "engine/pattern/Singleton.hpp"
#include "engine/pattern/Factory.hpp"
#include "engine/util/CocosUtil.hpp"

#include <string>
#include <functional>

class SceneFactory : public pattern::Singleton<SceneFactory>, public pattern::FactoryString<cocos2d::Scene>
{
public:
    SceneFactory();
};

//! Usage: CREATE_SCENE(ClassName);
#define CREATE_SCENE(_discreetClass) SceneFactory::getInstance().create(#_discreetClass)

//! Usage: REGISTER_SCENE(ClassName);
#define REGISTER_SCENE(_discreetClass) SceneFactory::getInstance().registerCreator(#_discreetClass, []() -> cocos2d::Scene* { return util::cocosutil::create<_discreetClass>(); })