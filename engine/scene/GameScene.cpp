#include "GameScene.hpp"

#include "SceneParser.hpp"
#include "SceneFactory.hpp"

#include "engine/debug/Log.hpp"
#include "engine/util/TypeCheck.h"

USING_NS_CC;

using namespace std;
using namespace debug;

const string GameScene::OVERLAY_SPRITE_NAME = "OVERLAY_SPRITE";
Sprite* GameScene::s_overlaySprite = nullptr;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::init()
{
    CHECK_IF_RETURN(!Scene::init(), false);

    // TODO: Backtround task
    string sceneFile = getSceneFile();
    if (!sceneFile.empty()) {
        CHECK_IF_RETURN(!loadSceneFile(sceneFile), false);
    }
    
    // TODO: Background task
    for (auto task : m_resourceTasks)
    {
        CHECK_IF_RETURN(!task(), false);
    }
    m_resourceTasks.clear();
    
    // TODO: Background task
    for (auto task : m_bgInitTasks)
    {
        CHECK_IF_RETURN(!task(), false);
    }
    m_bgInitTasks.clear();

    auto overlay = getOverlay();
    overlay->removeFromParent();
    
    return onInit();
}

void GameScene::cleanup()
{
    Scene::cleanup();

    CC_SAFE_RELEASE_NULL(m_fadeInEffect);
    CC_SAFE_RELEASE_NULL(m_fadeOutEffect);

    m_resourceTasks.clear();
    m_bgInitTasks.clear();

    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

void GameScene::onEnter()
{
    Scene::onEnter();
    scheduleUpdate();

    if (m_fadeInEffect)
    {
        runAction(m_fadeInEffect);
    }
}

void GameScene::onExit()
{
    Scene::onExit();
    // This should already be disabled when the scene begin to change.
    unscheduleUpdate();
}

Sprite* GameScene::getOverlay()
{
    if (!s_overlaySprite)
    {
        // Create overlay sprite
        s_overlaySprite = new Sprite();
        if (!s_overlaySprite->initWithFile("fill.png"))
        {
            LOG_WARNING("Failed to create overlay");
            return nullptr;
        }

        s_overlaySprite->setName(OVERLAY_SPRITE_NAME);
        s_overlaySprite->setContentSize(Director::getInstance()->getVisibleSize());
        s_overlaySprite->setLocalZOrder(INT_MAX);
        s_overlaySprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        s_overlaySprite->setVisible(false);
    }
    return s_overlaySprite;
}

void GameScene::enableTouch(bool enable)
{
    _eventDispatcher->removeEventListener(m_touchListener);
    m_touchListener = nullptr;

    if (enable)
    {
        CHECK_IF_MSG(isSceneChanging(), "Cannot enable touch during screen transitioning.");
        m_touchListener = EventListenerTouchOneByOne::create();
        m_touchListener->setSwallowTouches(true);
        m_touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
        m_touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
        m_touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
        m_touchListener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);

        _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);
    }
}
bool GameScene::isTouchEnabled()
{
    return m_touchListener != nullptr;
}

void GameScene::registerResourceLoadingTask(std::function<bool()> task)
{
    m_resourceTasks.push_back(task);
}

void GameScene::registerBgInitTask(std::function<bool()> task)
{
    m_bgInitTasks.push_back(task);
}

void GameScene::requestSpriteLoadingTask(const std::string& filePath)
{
    auto task = [filePath]() -> bool {
        Log::debug("[SpriteLoadingTask] Begin Loading sprite FILE=%s", filePath.c_str());
        if (loadSprite(filePath))
        {
            return true;
        }
        return false;
    };
    m_resourceTasks.push_back(task);
}

void GameScene::requestTextureLoadingTask(const std::string& filePath)
{
}

bool GameScene::loadSprite(const std::string& filePath)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    CHECK_IF_RETURN(fullPath.empty(), false);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filePath);
    Log::debug("[GameScene::loadSprite] Finished Loading sprite FILE=%s", filePath.c_str());
    return true;
}

bool GameScene::loadSceneFile(const std::string& filePath)
{
    SceneNodeParser parser(this);
    return parser.parseFile(filePath);
}

void GameScene::changeSceneByName(const std::string& sceneName, cocos2d::FiniteTimeAction* fadeInEffect, cocos2d::FiniteTimeAction* fadeOutEffect)
{
    changeSceneByName(sceneName, false, fadeInEffect, fadeOutEffect);
}

void GameScene::changeScene(GameScene* next)
{
    changeScene(next, false);
}

void GameScene::pushSceneByName(const std::string& sceneName, cocos2d::FiniteTimeAction* fadeInEffect, cocos2d::FiniteTimeAction* fadeOutEffect)
{
    changeSceneByName(sceneName, true, fadeInEffect, fadeOutEffect);
}

void GameScene::pushScene(GameScene* next)
{
    changeScene(next, true);
}

void GameScene::popScene()
{
    CHECK_IF_MSG(isSceneChanging(), "Cannot change to another scene during scene transitioning.");

    auto changeSceneCallback = [this] {
        LOG_INFO("Removing a scene NAME=%s from scene stack", getName().c_str());
        Director::getInstance()->popScene();
        
        enableTouch(false);
        unscheduleUpdate();
    };
    CallFunc* doSceneChange = CallFunc::create(changeSceneCallback);

    // Create an action sequence to trigger scene changing in the next update frame.
    FiniteTimeAction* sceneChangeAction = nullptr;

    if (m_fadeOutEffect)
    {
        // Perform fade out animations and then change the scene.
        sceneChangeAction = static_cast<FiniteTimeAction*>(Sequence::create(m_fadeOutEffect, doSceneChange, nullptr));
    }
    else
    {
        // Just directly change the scene in the next update frame.
        sceneChangeAction = static_cast<FiniteTimeAction*>(doSceneChange);
    }

    m_isSceneChanging = true;
    runAction(sceneChangeAction);
}

void GameScene::changeSceneByName(const std::string& sceneName, bool pushFlg, cocos2d::FiniteTimeAction* fadeInEffect, cocos2d::FiniteTimeAction* fadeOutEffect)
{
    GameScene* next = dynamic_cast<GameScene*>(SceneFactory::getInstance().create(sceneName));
    CHECK_IF_MSG(!next, "Invalid scene class NAME=%s", sceneName.c_str());

    if (fadeInEffect) {
        next->setFadeInEffect(fadeInEffect);
    }
    if (fadeOutEffect) {
        next->setFadeOutEffect(fadeOutEffect);
    }
    changeScene(next, pushFlg);
}

void GameScene::changeScene(GameScene* next, bool pushFlg)
{
    CHECK_IF_MSG(m_isSceneChanging, "Cannot change to another scene during scene transitioning.");

    CC_SAFE_RETAIN(next);

    auto changeSceneCallback = [this, next, pushFlg]() {
        Director* director = Director::getInstance();
        LOG_INFO("Scene is changing FROM=%s TO=%s", getName().c_str(), next->getName().c_str());

        next->init();

        if (pushFlg) {
            director->pushScene(next);
        }
        else {
            director->replaceScene(next);
        }
        CC_SAFE_RELEASE(next);

        enableTouch(false);
        unscheduleUpdate();
    };
    CallFunc* doSceneChange = CallFunc::create(changeSceneCallback);

    // Create an action sequence to trigger scene changing in the next update frame.
    FiniteTimeAction* sceneChangeAction = nullptr;

    if (m_fadeOutEffect)
    {
        // Perform fade out animations and then change the scene.
        sceneChangeAction = static_cast<FiniteTimeAction*>(Sequence::create(m_fadeOutEffect, doSceneChange, nullptr));
    }
    else
    {
        // Just directly change the scene in the next update frame.
        sceneChangeAction = static_cast<FiniteTimeAction*>(doSceneChange);
    }

    m_isSceneChanging = true;
    runAction(sceneChangeAction);
}

void GameScene::reloadScene()
{
    GameScene* tmpScene = new GameScene();
    tmpScene->setName("Step");
    tmpScene->autorelease();
    tmpScene->changeScene(this, true);

    changeScene(tmpScene, true);
}

bool GameScene::isSceneChanging() const
{
    return m_isSceneChanging;
}

void GameScene::setFadeInEffect(cocos2d::FiniteTimeAction* fadeInEffect)
{
    CC_SAFE_RELEASE_NULL(m_fadeInEffect);
    m_fadeInEffect = fadeInEffect;
    CC_SAFE_RETAIN(m_fadeInEffect);
}

void GameScene::setFadeOutEffect(cocos2d::FiniteTimeAction* fadeOutEffect)
{
    CC_SAFE_RELEASE_NULL(m_fadeOutEffect);
    m_fadeOutEffect = fadeOutEffect;
    CC_SAFE_RETAIN(m_fadeOutEffect);
}

void GameScene::setSceneChangeEffect(FiniteTimeAction* inAction, FiniteTimeAction* outAction)
{
    setFadeOutEffect(inAction);
    setFadeOutEffect(outAction);
}

void GameScene::setSceneChangeFadeEffect(Color3B color, float fadeTime)
{
    CC_SAFE_RELEASE_NULL(m_fadeOutEffect);
    CC_SAFE_RELEASE_NULL(m_fadeInEffect);

    Sprite* fadeOverlay = getOverlay();
    CHECK_IF_NULL(fadeOverlay);
    fadeOverlay->setColor(color);

    // Fade out
    {
        CallFunc* fadeFunc = CallFunc::create([this, fadeOverlay, fadeTime] {
            // We fade out the scene by fading in black overlay
            fadeOverlay->setVisible(true);
            fadeOverlay->setOpacity(0);
            fadeOverlay->runAction(FadeIn::create(fadeTime));

            if (!getChildByName(OVERLAY_SPRITE_NAME))
            {
                fadeOverlay->removeFromParent();
                addChild(fadeOverlay);
            }
        });
        DelayTime* delay = DelayTime::create(fadeTime);
        m_fadeOutEffect = Sequence::create(fadeFunc, delay, nullptr);
    }

    // Fade in
    {
        CallFunc* fadeFunc = CallFunc::create([this, fadeOverlay, fadeTime] {
            // We fade in the scene by fading out black overlay
            fadeOverlay->setVisible(true);
            fadeOverlay->setOpacity(0xFF);
            fadeOverlay->runAction(FadeOut::create(fadeTime));

            if (!getChildByName(OVERLAY_SPRITE_NAME))
            {
                fadeOverlay->removeFromParent();
                addChild(fadeOverlay);
            }
        });
        DelayTime* delay = DelayTime::create(fadeTime);
        m_fadeInEffect = Sequence::create(fadeFunc, delay, nullptr);
    }

    CC_SAFE_RETAIN(m_fadeOutEffect);
    CC_SAFE_RETAIN(m_fadeInEffect);
}
