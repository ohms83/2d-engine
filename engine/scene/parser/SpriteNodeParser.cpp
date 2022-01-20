#include "SpriteNodeParser.hpp"

#include "engine/debug/DebugMacros.h"
#include "engine/util/TypeCheck.h"

USING_NS_CC;
using namespace std;

SpriteNodeParser::SpriteNodeParser(Node* node)
: NodeParser(node)
{

}

bool SpriteNodeParser::parseAttributes(tinyxml2::XMLElement* element)
{
    Sprite* sprite = dynamic_cast<Sprite*>(m_node);
    CHECK_IF_RETURN_MSG(!sprite, false, "m_node is not a cocos2d::Sprite!");
    
    SET_XML_STR_ATTRIBUTE(element, "texture", sprite, initWithFile);
    SET_XML_STR_ATTRIBUTE(element, "frame", sprite, initWithSpriteFrameName);

    NodeParser::parseAttributes(element);
    return true;
}

bool SpriteNodeParser::parseElement(tinyxml2::XMLElement* element)
{
    const string elementName = element->Name();
    // It's recommended that <resources> should be the top most tags; to prevent initialization errors.
    if (elementName == "sliced")
    {
        return parseSliced(element);
    }
    else
    {
        return NodeParser::parseElement(element);
    }
}

bool SpriteNodeParser::parseSliced(tinyxml2::XMLElement* element)
{
    Sprite* sprite = dynamic_cast<Sprite*>(m_node);
    CHECK_IF_RETURN_MSG(!sprite, false, "m_node is not a cocos2d::Sprite!");
    
    Rect centerRect;
    element->QueryFloatAttribute("x", &centerRect.origin.x);
    element->QueryFloatAttribute("y", &centerRect.origin.y);
    element->QueryFloatAttribute("w", &centerRect.size.width);
    element->QueryFloatAttribute("h", &centerRect.size.height);

    sprite->setCenterRect(centerRect);
    return true;
}