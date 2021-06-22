#include "SceneParser.hpp"

#include "SceneFactory.hpp"
#include "GameScene.hpp"

#include "engine/debug/DebugMacros.h"
#include "engine/util/TypeCheck.h"

USING_NS_CC;
using namespace std;

#define IF_PARSE_TAG(_element, _tagName, _parseFunc) \
if (!strncmp(_element->Name(), _tagName, sizeof(_tagName))) \
{ \
    _parseFunc(_element); \
}

SceneParser::SceneParser(GameScene* scene)
{
    m_scene = scene;
    LOG_ASSERT(scene != nullptr, "scene cannot be NULL!")
}

bool SceneParser::parse(const std::string& sceneFile)
{
    tinyxml2::XMLDocument doc;
    
    const char* xmlFilePath = sceneFile.c_str();
    CHECK_IF_RETURN_MSG(
        doc.LoadFile(xmlFilePath) != tinyxml2::XML_SUCCESS,
        false,
        "Error loading file FILE=%s ERROR1=%s ERROR2=%s",
        xmlFilePath,
        doc.GetErrorStr1(),
        doc.GetErrorStr2());

    tinyxml2::XMLElement* sceneNode = doc.FirstChildElement("scene");
    CHECK_IF_NULL_RETURN_MSG(sceneNode, false, "Missing <scene> tag");

    tinyxml2::XMLElement* element = sceneNode->FirstChildElement();
    CHECK_IF_NULL_RETURN(element, false);
    return parse(element);
}
bool SceneParser::parse(tinyxml2::XMLElement* element)
{
    while (element)
    {
        // It's recommended that <resources> should be the top most tags; to prevent initialization errors.
        if (!strcmp(element->Name(), "resources"))
        {
            CHECK_IF_RETURN(!parseResources(element), false);
        }
        else if (!strcmp(element->Name(), "nodes"))
        {
            CHECK_IF_RETURN(!parseNodes(element), false);
        }
        
        element = element->NextSiblingElement();
    }
    return true;
}

bool SceneParser::parseResources(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLElement* element = root->FirstChildElement();
    while (element)
    {
        if (!strcmp(element->Name(), "spriteFrame"))
        {
            // TODO: Load sprite frame
        }
        else
        {
            LOG_ERROR("Unknown resource tag TAG=%s", element->Name());
            return false;
        }
        
        element = element->NextSiblingElement();
    }
    return true;
}

bool SceneParser::parseNodes(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLElement* element = root->FirstChildElement();
    while (element)
    {
        Node* node = nullptr;
        if (!strcmp(element->Name(), "sprite"))
        {
            SpriteNodeParser parser(m_scene);
            node = parser.parse(element);
            CHECK_IF_RETURN(!node, false);
        }
        else if (!strcmp(element->Name(), "node"))
        {
            NodeParser parser(m_scene);
            node = parser.parse(element);
            CHECK_IF_RETURN(!node, false);
        }
        
        element = element->NextSiblingElement();
    }
    return true;
}

const string NodeParser::ABSOLUTE_TYPE = "absolute";
const string NodeParser::RELATIVE_TYPE = "relative";

NodeParser::NodeParser(Node* parent)
{
    m_parent = parent;
    LOG_ASSERT(parent != nullptr, "parent cannot be NULL!")
}
Node* NodeParser::parse(tinyxml2::XMLElement* element)
{
    CHECK_IF_NULL_RETURN(element, nullptr);

    m_node = createNode();
    LOG_ASSERT(m_node != nullptr, "createNode returned nullptr!")

    parseAttributes(element);
    parseChildren(element);
    
    m_parent->addChild(m_node);
    return m_node;
}
void NodeParser::parseAttributes(tinyxml2::XMLElement* element)
{
    const char* name = element->Attribute("name");
    if (name) {
        m_node->setName(name);
    }
    m_node->setLocalZOrder(element->IntAttribute("zOrder"));
    m_node->setRotation(element->FloatAttribute("rotation"));
}
void NodeParser::parseChildren(tinyxml2::XMLElement* element)
{
    tinyxml2::XMLElement* child = element->FirstChildElement();
    
    while (child)
    {
        IF_PARSE_TAG(child, "position", parsePosition)
        else IF_PARSE_TAG(child, "size", parseSize)
        else IF_PARSE_TAG(child, "scale", parseScale)
        else IF_PARSE_TAG(child, "anchor", parseAnchor)
        else
        {
            LOG_WARNING("Unknown tag TAG=%s", child->Name());
        }
        
        child = child->NextSiblingElement();
    }
}
void NodeParser::parsePosition(tinyxml2::XMLElement* element)
{
    Vec2 pos;
    cocos2d::Size parentSize = m_parent->getContentSize();
    bool isRelative = checkIfRelative(element);
    
    element->QueryFloatAttribute("x", &pos.x);
    element->QueryFloatAttribute("y", &pos.y);
    
    if (isRelative)
    {
        pos.x *= parentSize.width;
        pos.y *= parentSize.height;
    }
    
    m_node->setPosition(pos);
}
void NodeParser::parseAnchor(tinyxml2::XMLElement* element)
{
    Vec2 anchor(0.5f, 0.5f);
    
    element->QueryFloatAttribute("x", &anchor.x);
    element->QueryFloatAttribute("y", &anchor.y);
    
    m_node->setAnchorPoint(anchor);
}
void NodeParser::parseScale(tinyxml2::XMLElement* element)
{
    Vec2 scale(1, 1);
    cocos2d::Size parentSize = m_parent->getContentSize();
    
    element->QueryFloatAttribute("x", &scale.x);
    element->QueryFloatAttribute("y", &scale.y);
    
    m_node->setScale(scale.x, scale.y);
}
void NodeParser::parseSize(tinyxml2::XMLElement* element)
{
    cocos2d::Size size;
    cocos2d::Size parentSize = m_parent->getContentSize();
    bool isRelative = checkIfRelative(element);
    
    element->QueryFloatAttribute("w", &size.width);
    element->QueryFloatAttribute("h", &size.height);
    
    if (isRelative)
    {
        size.width *= parentSize.width;
        size.height *= parentSize.height;
    }
    
    m_node->setContentSize(size);
}
Node* NodeParser::createNode()
{
    return Node::create();
}
bool NodeParser::checkIfRelative(tinyxml2::XMLElement* element)
{
    const char* type = element->Attribute("type");
    return (type != nullptr) && !strncmp(type, RELATIVE_TYPE.c_str(), RELATIVE_TYPE.size());
}

SpriteNodeParser::SpriteNodeParser(Node* parent)
: NodeParser(parent)
{

}
void SpriteNodeParser::parseAttributes(tinyxml2::XMLElement* element)
{
    Sprite* sprite = dynamic_cast<Sprite*>(m_node);
    LOG_ASSERT(sprite, "m_node is not a cocos2d::Sprite!");

    const char* texturePath = element->Attribute("texture");
    if (texturePath) {
        sprite->initWithFile(texturePath);
    }

    NodeParser::parseAttributes(element);
}
Node* SpriteNodeParser::createNode()
{
    return Sprite::create();
}