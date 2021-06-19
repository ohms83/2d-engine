#include "GameScene.hpp"

#include "engine/debug/Log.hpp"
#include "engine/util/TypeCheck.h"

USING_NS_CC;

using namespace std;
using namespace debug;

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
    
    // TODO: Background task
    for (auto task : m_bgInitTasks)
    {
        CHECK_IF_RETURN(!task(), false);
    }
    
    return onInit();
}

void GameScene::onEnter()
{
    Scene::onEnter();
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
    tinyxml2::XMLDocument doc;
    
    const char* xmlFilePath = filePath.c_str();
    CHECK_IF_RETURN_MSG(doc.LoadFile(xmlFilePath) != tinyxml2::XML_SUCCESS, false, "Error loading file FILE=%s", xmlFilePath);

    tinyxml2::XMLElement* sceneNode = doc.FirstChildElement("scene");
    CHECK_IF_NULL_RETURN_MSG(sceneNode, false, "Missing <scene> tag");

    tinyxml2::XMLElement* element = sceneNode->FirstChildElement();
    CHECK_IF_NULL_RETURN(element, false);
    return parseScene(element, this);
}

bool GameScene::parseScene(tinyxml2::XMLElement* element, cocos2d::Node* currentNode)
{
    while (element)
    {
        // It's recommended that <resource> should be the top most tags; to prevent initialization errors.
        if (!strcmp(element->Name(), "resource"))
        {
            CHECK_IF_RETURN(!parseResourceTag(element), false);
        }
        else if (!strcmp(element->Name(), "sprite"))
        {
            Sprite* sprite = Sprite::create();
            CHECK_IF_RETURN(!parseSpriteTag(element, sprite, currentNode), false);
        }
        else if (!strcmp(element->Name(), "node"))
        {
            Node* node = Node::create();
            CHECK_IF_RETURN(!parseNodeTag(element, node, currentNode), false);
        }
        
        element = element->NextSiblingElement();
    }
    return true;
}

bool GameScene::parseResourceTag(tinyxml2::XMLElement* element)
{
    const char* type = element->Attribute("type");
    const char* path = element->GetText();
    CHECK_IF_NULL_RETURN(type, false);
    CHECK_IF_NULL_RETURN(path, false);
    
    if (!strcmp(type, "sprite"))
    {
        loadSprite(path);
    }
    else if (!strcmp(type, "texture"))
    {
        // TODO:
    }
    else
    {
        LOG_ERROR("Unknown resource type TYPE=%s PATH=%s", type, path);
        return false;
    }
    
    return true;
}

bool GameScene::parseNodeTag(tinyxml2::XMLElement* element, Node* node, cocos2d::Node* parent)
{
    CHECK_IF_NULL_RETURN(node, false);
    CHECK_IF_NULL_RETURN(parent, false);
    
    // --- Set node's attributes ---
    const char* name = element->Attribute("name");
    if (name) {
        node->setName(name);
    }
    
    node->setLocalZOrder(element->IntAttribute("zorder"));
    // -----------------------------
    
    tinyxml2::XMLElement* child = element->FirstChildElement();
    
    while (child)
    {
        if (!strcmp(child->Name(), "position"))
        {
            parsePosition(child, node, parent);
        }
        else if (!strcmp(child->Name(), "size"))
        {
            parseSize(child, node, parent);
        }
        else if (!strcmp(child->Name(), "scale"))
        {
            parseScale(child, node);
        }
        else if (!strcmp(child->Name(), "rotation"))
        {
            parseRotation(child, node);
        }
        
        child = child->NextSiblingElement();
    }
    
    parent->addChild(node);
    return true;
}

bool GameScene::parseSpriteTag(tinyxml2::XMLElement* element, Sprite* sprite, cocos2d::Node* parent)
{
    CHECK_IF_NULL_RETURN(sprite, false);
    CHECK_IF_NULL_RETURN(parent, false);
    
    static SpriteFrameCache* spriteFrameCache = SpriteFrameCache::getInstance();
    
    const char* frameName = element->Attribute("frame");
    const char* textureName = element->Attribute("texture");
    
    if (frameName)
    {
        SpriteFrame* spriteFrame = spriteFrameCache->getSpriteFrameByName(frameName);
        if (spriteFrame)
        {
            sprite->initWithSpriteFrame(spriteFrame);
        }
        else
        {
            LOG_ERROR("[%s] Invalid sprite frame NAME=%s", frameName);
        }
    }
    else if (textureName)
    {
        if (!sprite->initWithFile(textureName))
        {
            LOG_ERROR("[%s] Invalid texture FILE=%s", textureName);
        }
    }
    
    return parseNodeTag(element, sprite, parent);
}

void GameScene::parsePosition(tinyxml2::XMLElement* element, Node* node, cocos2d::Node* parent)
{
    Vec2 position;
    Size parentSize = parent->getContentSize();
    
    // Absolute value has higher priority than relative
    if (element->QueryFloatAttribute("x", &position.x) != tinyxml2::XML_NO_ERROR)
    {
        element->QueryFloatAttribute("relX", &position.x);
        position.x *= parentSize.width;
    }
    if (element->QueryFloatAttribute("y", &position.y) != tinyxml2::XML_NO_ERROR)
    {
        element->QueryFloatAttribute("relY", &position.y);
        position.y *= parentSize.height;
    }
    
    node->setPosition(position);
}

void GameScene::parseSize(tinyxml2::XMLElement* element, Node* node, cocos2d::Node* parent)
{
    Size size;
    Size parentSize = parent->getContentSize();
    
    // Absolute value has higher priority than relative
    if (element->QueryFloatAttribute("w", &size.width) != tinyxml2::XML_NO_ERROR)
    {
        element->QueryFloatAttribute("relW", &size.width);
        size.width *= parentSize.width;
    }
    if (element->QueryFloatAttribute("h", &size.height) != tinyxml2::XML_NO_ERROR)
    {
        element->QueryFloatAttribute("relH", &size.height);
        size.height *= parentSize.height;
    }
    
    node->setContentSize(size);
}

void GameScene::parseScale(tinyxml2::XMLElement* element, Node* node)
{
    Vec2 scale;
    scale.x = element->FloatAttribute("x");
    scale.y = element->FloatAttribute("y");
    node->setScale(scale.x, scale.y);
}

void GameScene::parseRotation(tinyxml2::XMLElement* element, Node* node)
{
    node->setRotation(element->FloatAttribute("degree"));
}
