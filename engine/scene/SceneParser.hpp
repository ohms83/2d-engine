#pragma once

#include "cocos2d.h"
#include "external/tinyxml2/tinyxml2.h"

class GameScene;

class SceneParser
{
public:
    static bool parseSceneFile(const std::string& sceneFile, GameScene* scene);
};

class NodeParser
{
public:
    NodeParser(cocos2d::Node* parent);
    virtual cocos2d::Node* parse(tinyxml2::XMLElement* element);

    /**
     *  Used in @c type property of @c <position> and @c <size> tags. When data type is @c absolute
     *  the position and size parameters will be used as-is. When the type is undefined, the parameter
     *  will always be assumed this type.
     */
    static const std::string ABSOLUTE_TYPE;
    /**
     *  Used in @c type property of @c <position> and @c <size> tags. When data type is @c relative
     *  the position and size parameters are proportions of the node's parent.
     *  For example, x="0.5" means the node's X coordinate is at the center of it's parent.
     */
    static const std::string RELATIVE_TYPE;

protected:
    /// Parse node's attributes
    virtual void parseAttributes(tinyxml2::XMLElement* element);
    /// Parse child elements
    virtual void parseChildren(tinyxml2::XMLElement* element);
    /**
     * Inspect the tag name and parse the given element accordingly.
     * @return @c true , if the element's tag name can be recognised and correctly parsed; otherwise, @c false
     */
    virtual bool parseElement(tinyxml2::XMLElement* element);
    /// Parse @c position element
    void parsePosition(tinyxml2::XMLElement* element);
    /// Parse @c scale element
    void parseScale(tinyxml2::XMLElement* element);
    /// Parse @c size element
    void parseSize(tinyxml2::XMLElement* element);
    /// Parse @c anchor element
    void parseAnchor(tinyxml2::XMLElement* element);
    /// Parse @c color element
    void parseColor(tinyxml2::XMLElement* element);

    virtual cocos2d::Node* createNode();

    static bool checkIfRelative(tinyxml2::XMLElement* element);

protected:
    cocos2d::Node* m_parent = nullptr;
    cocos2d::Node* m_node = nullptr;
};

class SceneNodeParser : public NodeParser
{
public:
    SceneNodeParser(GameScene* scene);
protected:
    cocos2d::Node* createNode() override;

    bool parseElement(tinyxml2::XMLElement* element) override;
    bool parseResources(tinyxml2::XMLElement* element);

protected:
    GameScene* m_scene = nullptr;
};

class SpriteNodeParser : public NodeParser
{
public:
    SpriteNodeParser(cocos2d::Node* parent);

protected:
    void parseAttributes(tinyxml2::XMLElement* element) override;
    cocos2d::Node* createNode() override;
};