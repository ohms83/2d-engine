#pragma once

#include "cocos2d.h"
#include "external/tinyxml2/tinyxml2.h"

#include <string>
#include <vector>
#include <functional>

class GameScene : public cocos2d::Scene
{
public:
    GameScene();
    virtual ~GameScene();

    /**
     *  Initialize current scene.
     *  @discussion All the default initialization processes will be initiated here (loading scene file, running init tasks, etc.).
     *              It will also run child classes's @c onInit funciton as the part of initialization.
     *              The initialization orders are as of the following
     *                  1. Resource loading
     *                  2. Background initialization tasks
     *                  3. @c onInit
     *  @return @c false if there's any error during initialization (including the error from child class's @c onInit );
     *          otherwise, @c true .
     */
    bool init() final;
    void cleanup() override;
    
    void registerResourceLoadingTask(std::function<bool()> task);
    void registerBgInitTask(std::function<bool()> task);
    /**
     *  Request for loading the given sprite during initialization
     *  @param filePath Path to <spriteName>.plist
     */
    void requestSpriteLoadingTask(const std::string& filePath);
    /// Request for loading the given texture during initialization
    void requestTextureLoadingTask(const std::string& filePath);
    
    static bool loadSprite(const std::string& filePath);
    
    void onEnter() override;
    void onExit() override;

    void enableTouch(bool enable);
    bool isTouchEnabled();

    /**
     * Replace current scene with the specified scene.
     * @param sceneName Name of scene class registered with @c SceneFactory .
     * @param fadeInEffect Special effect used when transitioning to the new scene.
     * @param fadeOutEffect Special effect used when transitioning out from this scene.
     */
    void changeSceneByName(const std::string& sceneName, cocos2d::FiniteTimeAction* fadeInEffect = nullptr, cocos2d::FiniteTimeAction* fadeOutEffect = nullptr);
    /// Replace current scene with the specified scene.
    void changeScene(GameScene* next);

    /**
     * Replace current scene with the specified scene.
     * @param sceneName Name of scene class registered with @c SceneFactory .
     * @param fadeInEffect Special effect used when transitioning to the new scene.
     * @param fadeOutEffect Special effect used when transitioning out from this scene.
     */
    void pushSceneByName(const std::string& sceneName, cocos2d::FiniteTimeAction* fadeInEffect = nullptr, cocos2d::FiniteTimeAction* fadeOutEffect = nullptr);
    /// Push the specified scene to scene stack and suspend current scene.
    void pushScene(GameScene* next);
    /// Exit from the current scene and returned to the previous one in scene stack.
    void popScene();

    /// Temporary switch to and empty scene and then switch back.
    void reloadScene();

    bool isSceneChanging() const;

    void setFadeInEffect(cocos2d::FiniteTimeAction* fadeInEffect);
    void setFadeOutEffect(cocos2d::FiniteTimeAction* fadeOutEffect);

    /**
     * Set screen transitioning effect.
     * @param inAction Special effect using when transitioning 'in' to this scene
     * @param outAction Special effect using when transitioning 'out' from this scene
     */
    void setSceneChangeEffect(cocos2d::FiniteTimeAction* inAction, cocos2d::FiniteTimeAction* outAction);
    /// Set screen transitioning effect to fade in/out; which is the most commonly used effect.
    void setSceneChangeFadeEffect(cocos2d::Color3B color = cocos2d::Color3B::BLACK, float fadeTime = 0.3f);

    /**
     * Overlay sprite is the top-most sprite that can be used for various effects; for example, full-screen fading or color tinting.
     * Every scenes will have one overlay of their own.
     */
    static const std::string OVERLAY_SPRITE_NAME;
    static cocos2d::Sprite* getOverlay();

protected:
    /// @c onInit will be called at the final step of scene initailization process.
    virtual bool onInit() { return true; }
    /// Use this to specify which scene file to be loaded during initialization. Leave it empty to not load the scene file.
    virtual std::string getSceneFile() { return ""; }

    /// Load and initialize the scene from specified XML scene file. 
    bool loadSceneFile(const std::string& filePath);

    void changeSceneByName(const std::string& sceneName, bool pushFlg, cocos2d::FiniteTimeAction* fadeInEffect, cocos2d::FiniteTimeAction* fadeOutEffect);
    void changeScene(GameScene* next, bool pushFlg);

    /**
     * @return @c true , to consume the event and stop propagation; otherwise, @c false
     */
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
    {
        return false;
    }
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {}
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {}
    virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event) {}

protected:
    /// A list of resource loading tasks.
    std::vector<std::function<bool()>> m_resourceTasks;
    /**
     *  A list of tasks that will be run in background during initialzation. All the tasks will run in parallel, so, they MUST
     *  NOT have any dependencies toward each others. Any initialization tasks that expected to be time consuming should be
     *  put inside here.
     */
    std::vector<std::function<bool()>> m_bgInitTasks;

    /// Special effect used when transitioning to scene.
    cocos2d::FiniteTimeAction* m_fadeInEffect = nullptr;
    /// Special effect used when transitioning from this scene to the next.
    cocos2d::FiniteTimeAction* m_fadeOutEffect = nullptr;

private:
    bool m_isSceneChanging = false;
    cocos2d::EventListenerTouchOneByOne* m_touchListener = nullptr;

    /// Shared overlay sprite for all scenes. This shouldn't be removed by any scene.
    static cocos2d::Sprite* s_overlaySprite;
};
