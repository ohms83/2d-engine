#include "NodeParser.hpp"

#include "LabelNodeParser.hpp"
#include "SpriteNodeParser.hpp"

#include "engine/debug/DebugMacros.h"
#include "engine/util/TypeCheck.h"
#include "engine/util/CocosUtil.hpp"

USING_NS_CC;
using namespace std;

const string NodeParser::ABSOLUTE_TYPE = "absolute";
const string NodeParser::RELATIVE_TYPE = "relative";

NodeParser::NodeParser(Node* node)
{
    LOG_ASSERT(node, "node cannot be NULL");
    m_node = node;
    m_parent = node->getParent();
}

void NodeParser::setInvertY(bool flag)
{
    m_invertY = flag;
}

bool NodeParser::isInvertY() const
{
    return m_invertY;
}

bool NodeParser::parseFile(const std::string& file)
{
    tinyxml2::XMLDocument doc;
    
    const char* xmlFilePath = file.c_str();
    std::string fileData = std::move(cocos2d::FileUtils::getInstance()->getStringFromFile(xmlFilePath));
    CHECK_IF_RETURN_MSG(
        doc.Parse(fileData.c_str(), fileData.size()) != tinyxml2::XML_SUCCESS,
        false,
        "Error loading file FILE=%s ERROR1=%s ERROR2=%s",
        xmlFilePath,
        doc.GetErrorStr1(),
        doc.GetErrorStr2());

    const std::string rootTag = getRootTag();
    tinyxml2::XMLElement* sceneNode = doc.FirstChildElement(rootTag.c_str());
    CHECK_IF_NULL_RETURN_MSG(sceneNode, false, "Missing %s tag", rootTag.c_str());

    return parse(sceneNode);
}

bool NodeParser::parse(tinyxml2::XMLElement* element)
{
    CHECK_IF_NULL_RETURN(element, false);

    if (!parseAttributes(element) || !parseChildren(element))
    {
        LOG_ERROR("Error parsing scene node TAG=%s", getRootTag().c_str());
        return false;
    }
    return true;
}

bool NodeParser::parseAttributes(tinyxml2::XMLElement* element)
{
    const char* templateFile = element->Attribute("template");
    if (templateFile && !parseFile(templateFile))
    {
        return false;
    }

    SET_XML_STR_ATTRIBUTE(element, "name", m_node, setName);
    SET_XML_INT_ATTRIBUTE(element, "zOrder", m_node, setLocalZOrder);
    SET_XML_FLOAT_ATTRIBUTE(element, "rotation", m_node, setRotation);
    SET_XML_INT_ATTRIBUTE(element, "opacity", m_node, setOpacity);
    SET_XML_BOOL_ATTRIBUTE(element, "cascadeColorEnabled", m_node, setCascadeColorEnabled);
    SET_XML_BOOL_ATTRIBUTE(element, "cascadeOpacityEnabled", m_node, setCascadeOpacityEnabled);
    
    element->QueryBoolAttribute("debugDraw", &m_debugDraw);

    return true;
}

bool NodeParser::parseChildren(tinyxml2::XMLElement* element)
{
    tinyxml2::XMLElement* child = element->FirstChildElement();
    
    while (child)
    {
        if (!parseElement(child))
        {
            LOG_WARNING("Error parsing TAG=%s", child->Name());
            return false;
        }
        
        child = child->NextSiblingElement();
    }

    if (m_debugDraw) {
        util::cocosutil::attachDebugDrawToNode(m_node);
    }
    return true;
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
    else if (elementName == "node")
    {
        parseChildElement<NodeParser, Node>(element, isInvertY());
    }
    else if (elementName == "sprite")
    {
        parseChildElement<SpriteNodeParser, Sprite>(element, isInvertY());
    }
    else if (elementName == "label")
    {
        parseChildElement<LabelNodeParser, Label>(element, isInvertY());
    }
    else if (elementName == "ui")
    {
        parseChildElement<NodeParser, Node>(element, true);
    }
    else
    {
        LOG_WARNING("Unknown tag TAG=%s", elementName.c_str());
        return false;
    }
    
    if (childNode && childNode->getParent() == nullptr) {
        m_node->addChild(childNode);
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
    
    if (isInvertY())
    {
        cocos2d::Size parentSize = (m_parent ? m_parent->getContentSize() : cocos2d::Size::ZERO);
        pos = util::cocosutil::inverseY(pos, parentSize);
    }
    setPosition(pos);
}

void NodeParser::parseAnchor(tinyxml2::XMLElement* element)
{
    Vec2 anchor(0.5f, 0.5f);
    
    element->QueryFloatAttribute("x", &anchor.x);
    element->QueryFloatAttribute("y", &anchor.y);
    
    setAnchor(anchor);
}

void NodeParser::parseScale(tinyxml2::XMLElement* element)
{
    Vec2 scale(1, 1);
    float value = 1;
    
    if (tinyxml2::XML_NO_ERROR == element->QueryFloatAttribute("value", &value))
    {
        scale.x = value;
        scale.y = value;
    }
    else
    {
        element->QueryFloatAttribute("x", &scale.x);
        element->QueryFloatAttribute("y", &scale.y);
    }
    
    setScale(scale);
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
    
    setSize(size);
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
    setColor(color);
}

void NodeParser::setPosition(const cocos2d::Vec2& pos)
{
    m_node->setPosition(pos);
}

void NodeParser::setScale(const cocos2d::Vec2& scale)
{
    m_node->setScale(scale.x, scale.y);
}

void NodeParser::setSize(const cocos2d::Size& size)
{
    m_node->setContentSize(size);
}

void NodeParser::setAnchor(const cocos2d::Vec2& anchor)
{
    m_node->setAnchorPoint(anchor);
}

void NodeParser::setColor(const cocos2d::Color3B& color)
{
    m_node->setColor(color);
}

bool NodeParser::checkIfRelative(tinyxml2::XMLElement* element)
{
    const char* type = element->Attribute("type");
    return (type != nullptr) && !strncmp(type, RELATIVE_TYPE.c_str(), RELATIVE_TYPE.size());
}