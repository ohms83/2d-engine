#include "SceneFactory.hpp"

#include "engine/util/CocosUtil.hpp"

#include "AllScenes.h"

// TODO: This is an example scene and should be removed from the actual project.
#include "HelloWorldScene.h"

USING_NS_CC;

using namespace std;
using namespace pattern;

//! Usage: REGISTER_SCENE(ClassName);
#define REGISTER_SCENE(_discreetClass) registerCreator(#_discreetClass, []() -> cocos2d::Scene* { return util::cocosutil::createAndInit<_discreetClass>(); })

SceneFactory::SceneFactory()
{
    // Register all the scene classes here
    // TODO: This is an example scene and should be removed from the actual project.
    REGISTER_SCENE(HelloWorld);
}