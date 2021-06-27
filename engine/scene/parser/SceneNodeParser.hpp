#pragma once

#include "NodeParser.hpp"

class GameScene;

class SceneNodeParser : public NodeParser
{
public:
    SceneNodeParser(GameScene* scene);

    const std::string getRootTag() override
    {
        return "scene";
    }

protected:
    cocos2d::Node* createNode() override;

    bool parseElement(tinyxml2::XMLElement* element) override;
    bool parseResources(tinyxml2::XMLElement* element);

protected:
    GameScene* m_scene = nullptr;
    /// Cached cocos2d::Director's singleton
    cocos2d::Director* m_director = nullptr;
};