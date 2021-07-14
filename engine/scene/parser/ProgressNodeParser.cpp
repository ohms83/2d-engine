#include "ProgressNodeParser.hpp"

#include "engine/debug/DebugMacros.h"
#include "engine/util/TypeCheck.h"

USING_NS_CC;
using namespace std;

const string ProgressNodeParser::RADIAL_TYPE = "radial";
const string ProgressNodeParser::BAR_TYPE = "bar";

ProgressNodeParser::ProgressNodeParser(Node* node)
: NodeParser(node)
{
}

bool ProgressNodeParser::parseAttributes(tinyxml2::XMLElement* element)
{
    ProgressTimer* progress = dynamic_cast<ProgressTimer*>(m_node);
    CHECK_IF_RETURN_MSG(!progress, false, "m_node is not a cocos2d::ProgressTimer!");
    
    Sprite* sprite = Sprite::create();
    SET_XML_STR_ATTRIBUTE(element, "texture", sprite, initWithFile);
    SET_XML_STR_ATTRIBUTE(element, "frame", sprite, initWithSpriteFrameName);
    SET_XML_FLOAT_ATTRIBUTE(element, "percent", progress, setPercentage);

    ProgressTimer::Type barType = ProgressTimer::Type::BAR;
    const char* type = element->Attribute("type");
    if (type)
    {
        if (strncmp(type, RADIAL_TYPE.c_str(), RADIAL_TYPE.size()) == 0) {
            barType = ProgressTimer::Type::RADIAL;
        }
        else if (strncmp(type, BAR_TYPE.c_str(), BAR_TYPE.size()) == 0) {
            barType = ProgressTimer::Type::BAR;
        }
        else {
            LOG_WARNING("Unknown progress-bar type TYPE=%s", type);
            return false;
        }
    }

    progress->initWithSprite(sprite);
    progress->setType(barType);

    NodeParser::parseAttributes(element);
    return true;
}

bool ProgressNodeParser::parseElement(tinyxml2::XMLElement* element)
{
    const string elementName = element->Name();
    if (elementName == "midPoint")
    {
        return parseMidPoint(element);
    }
    if (elementName == "changeRate")
    {
        return parseBarChangeRate(element);
    }
    else
    {
        return NodeParser::parseElement(element);
    }
}

void ProgressNodeParser::setSize(const cocos2d::Size& size)
{
    ProgressTimer* progress = dynamic_cast<ProgressTimer*>(m_node);
    CHECK_IF_MSG(!progress, "m_node is not a cocos2d::ProgressTimer!");

    Sprite* sprite = progress->getSprite();
    CHECK_IF_MSG(!sprite, "Missing progress bar sprite. Please make sure that either 'texture' or 'frame' attribute was assigned.");

    sprite->setContentSize(size);
    progress->setContentSize(size);
}

bool ProgressNodeParser::parseMidPoint(tinyxml2::XMLElement* element)
{
    ProgressTimer* progress = dynamic_cast<ProgressTimer*>(m_node);
    CHECK_IF_RETURN_MSG(!progress, false, "m_node is not a cocos2d::ProgressTimer!");

    Vec2 point(0.0f, 0.0f);
    
    element->QueryFloatAttribute("x", &point.x);
    element->QueryFloatAttribute("y", &point.y);
    
    progress->setMidpoint(point);
    return true;
}

bool ProgressNodeParser::parseBarChangeRate(tinyxml2::XMLElement* element)
{
    ProgressTimer* progress = dynamic_cast<ProgressTimer*>(m_node);
    CHECK_IF_RETURN_MSG(!progress, false, "m_node is not a cocos2d::ProgressTimer!");

    Vec2 rate(0.0f, 0.0f);
    
    element->QueryFloatAttribute("x", &rate.x);
    element->QueryFloatAttribute("y", &rate.y);
    
    progress->setBarChangeRate(rate);
    return true;

}