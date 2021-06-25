#include "SceneParser.hpp"

#include "SceneFactory.hpp"
#include "GameScene.hpp"

#include "engine/debug/DebugMacros.h"
#include "engine/util/TypeCheck.h"

USING_NS_CC;
using namespace std;

#define SET_ATTRIBUTE(_element, _type, _name, _getter, _node, _setter) { \
    _type value; \
    if (tinyxml2::XML_NO_ERROR == _element->Query##_getter(_name, &value)) { \
        _node->_setter(value); \
    } \
}

#define SET_STR_ATTRIBUTE(_element, _name, _node, _setter) { \
    const char* value = _element->Attribute(_name); \
    if (value) { \
        _node->_setter(value); \
    } \
}
#define SET_INT_ATTRIBUTE(_element, _name, _node, _setter) SET_ATTRIBUTE(_element, int, _name, IntAttribute, _node, _setter)
#define SET_FLOAT_ATTRIBUTE(_element, _name, _node, _setter) SET_ATTRIBUTE(_element, float, _name, FloatAttribute, _node, _setter)
#define SET_BOOL_ATTRIBUTE(_element, _name, _node, _setter) SET_ATTRIBUTE(_element, bool, _name, BoolAttribute, _node, _setter)

bool SceneParser::parseSceneFile(const std::string& sceneFile, GameScene* scene)
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

    SceneNodeParser parser(scene);
    return parser.parse(sceneNode) != nullptr;
}

const string NodeParser::ABSOLUTE_TYPE = "absolute";
const string NodeParser::RELATIVE_TYPE = "relative";

NodeParser::NodeParser(Node* parent)
{
    m_parent = parent;
}
Node* NodeParser::parse(tinyxml2::XMLElement* element)
{
    CHECK_IF_NULL_RETURN(element, nullptr);

    m_node = createNode();
    LOG_ASSERT(m_node != nullptr, "createNode returned nullptr!")

    parseAttributes(element);
    parseChildren(element);
    
    if (m_parent) {
        m_parent->addChild(m_node);
    }
    return m_node;
}
void NodeParser::parseAttributes(tinyxml2::XMLElement* element)
{
    SET_STR_ATTRIBUTE(element, "name", m_node, setName);
    SET_INT_ATTRIBUTE(element, "zOrder", m_node, setLocalZOrder);
    SET_FLOAT_ATTRIBUTE(element, "rotation", m_node, setRotation);
    SET_INT_ATTRIBUTE(element, "opacity", m_node, setOpacity);
    SET_BOOL_ATTRIBUTE(element, "cascadeColorEnabled", m_node, setCascadeColorEnabled);
    SET_BOOL_ATTRIBUTE(element, "cascadeOpacityEnabled", m_node, setCascadeOpacityEnabled);
}
void NodeParser::parseChildren(tinyxml2::XMLElement* element)
{
    tinyxml2::XMLElement* child = element->FirstChildElement();
    
    while (child)
    {
        if (!parseElement(child))
        {
            LOG_WARNING("Error parsing TAG=%s", child->Name());
            return;
        }
        
        child = child->NextSiblingElement();
    }
}
bool NodeParser::parseElement(tinyxml2::XMLElement* element)
{
    const string elementName = element->Name();
    Node* childNode = nullptr;

    if (elementName == "position")
    {
        parsePosition(element);
    }
    else if (elementName == "size")
    {
        parseSize(element);
    }
    else if (elementName == "scale")
    {
        parseScale(element);
    }
    else if (elementName == "anchor")
    {
        parseAnchor(element);
    }
    else if (elementName == "color")
    {
        parseColor(element);
    }
    else if (elementName == "sprite")
    {
        SpriteNodeParser parser(m_node);
        childNode = parser.parse(element);
        CHECK_IF_RETURN(!childNode, false);
    }
    else if (elementName == "node")
    {
        NodeParser parser(m_node);
        childNode = parser.parse(element);
        CHECK_IF_RETURN(!childNode, false);
    }
    else
    {
        LOG_WARNING("Unknown tag TAG=%s", elementName.c_str());
        return false;
    }

    return true;
}
void NodeParser::parsePosition(tinyxml2::XMLElement* element)
{
    Vec2 pos;
    cocos2d::Size parentSize = (m_parent ? m_parent->getContentSize() : cocos2d::Size::ZERO);
    bool isRelative = checkIfRelative(element);
    
    element->QueryFloatAttribute("x", &pos.x);
    element->QueryFloatAttribute("y", &pos.y);
    
    if (isRelative)
    {
        if (!parentSize.equals(cocos2d::Size::ZERO))
        {
            pos.x *= parentSize.width;
            pos.y *= parentSize.height;
        }
        else
        {
            LOG_WARNING("Node's attribute is of type 'relative' while the parent's size is ZERO");
        }
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
    
    element->QueryFloatAttribute("x", &scale.x);
    element->QueryFloatAttribute("y", &scale.y);
    
    m_node->setScale(scale.x, scale.y);
}
void NodeParser::parseSize(tinyxml2::XMLElement* element)
{
    cocos2d::Size size;
    cocos2d::Size parentSize = (m_parent ? m_parent->getContentSize() : cocos2d::Size::ZERO);
    bool isRelative = checkIfRelative(element);
    
    element->QueryFloatAttribute("w", &size.width);
    element->QueryFloatAttribute("h", &size.height);
    
    if (isRelative)
    {
        if (!parentSize.equals(cocos2d::Size::ZERO))
        {
            size.width *= parentSize.width;
            size.height *= parentSize.height;
        }
        else
        {
            LOG_WARNING("Node's attribute is of type 'relative' while the parent's size is ZERO");
        }
    }
    
    m_node->setContentSize(size);
}
void NodeParser::parseColor(tinyxml2::XMLElement* element)
{
    Color3B color = Color3B::WHITE;
    
    uint32_t r, g, b;
    element->QueryUnsignedAttribute("r", &r);
    element->QueryUnsignedAttribute("g", &g);
    element->QueryUnsignedAttribute("b", &b);

    color.r = (uint8_t)r;
    color.g = (uint8_t)g;
    color.b = (uint8_t)b;
    m_node->setColor(color);
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

SceneNodeParser::SceneNodeParser(GameScene* scene)
: NodeParser(nullptr)
{
    LOG_ASSERT(scene != nullptr, "scene cannot be NULL!");
    m_scene = scene;
}
Node* SceneNodeParser::createNode()
{
    return static_cast<cocos2d::Node*>(m_scene);
}
bool SceneNodeParser::parseElement(tinyxml2::XMLElement* element)
{
    const string elementName = element->Name();
    // It's recommended that <resources> should be the top most tags; to prevent initialization errors.
    if (elementName == "resources")
    {
        return parseResources(element);
    }
    else
    {
        return NodeParser::parseElement(element);
    }
}
bool SceneNodeParser::parseResources(tinyxml2::XMLElement* root)
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

SpriteNodeParser::SpriteNodeParser(Node* parent)
: NodeParser(parent)
{

}
void SpriteNodeParser::parseAttributes(tinyxml2::XMLElement* element)
{
    Sprite* sprite = dynamic_cast<Sprite*>(m_node);
    LOG_ASSERT(sprite, "m_node is not a cocos2d::Sprite!");
    
    SET_STR_ATTRIBUTE(element, "texture", sprite, initWithFile);

    NodeParser::parseAttributes(element);
}
Node* SpriteNodeParser::createNode()
{
    return Sprite::create();
}