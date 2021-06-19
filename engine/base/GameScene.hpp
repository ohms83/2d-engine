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
    
    virtual void onEnter() override;

protected:
    /// @c onInit will be called at the final step of scene initailization process.
    virtual bool onInit() { return true; }
    /// Use this to specify which scene file to be loaded during initialization. Leave it empty to not load the scene file.
    virtual std::string getSceneFile() { return ""; }

    /// Load and initialize the scene from specified XML scene file. 
    bool loadSceneFile(const std::string& filePath);

    bool parseScene(tinyxml2::XMLElement* element, cocos2d::Node* currentNode);
    bool parseResourceTag(tinyxml2::XMLElement* element);
    
    bool parseNodeTag(tinyxml2::XMLElement* element, cocos2d::Node* node, cocos2d::Node* parent);
    bool parseSpriteTag(tinyxml2::XMLElement* element, cocos2d::Sprite* sprite, cocos2d::Node* parent);

    void parsePosition(tinyxml2::XMLElement* element, cocos2d::Node* node, cocos2d::Node* parent);
    void parseSize(tinyxml2::XMLElement* element, cocos2d::Node* node, cocos2d::Node* parent);
    void parseScale(tinyxml2::XMLElement* element, cocos2d::Node* node);
    void parseRotation(tinyxml2::XMLElement* element, cocos2d::Node* node);

protected:
    /// A list of resource loading tasks.
    std::vector<std::function<bool()>> m_resourceTasks;
    /**
     *  A list of tasks that will be run in background during initialzation. All the tasks will run in parallel, so, they MUST
     *  NOT have any dependencies toward each others. Any initialization tasks that expected to be time consuming should be
     *  put inside here.
     */
    std::vector<std::function<bool()>> m_bgInitTasks;
};
