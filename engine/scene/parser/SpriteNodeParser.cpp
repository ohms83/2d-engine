#include "SpriteNodeParser.hpp"

#include "engine/debug/DebugMacros.h"
#include "engine/util/TypeCheck.h"

USING_NS_CC;
using namespace std;

SpriteNodeParser::SpriteNodeParser(Node* parent)
: NodeParser(parent)
{

}
void SpriteNodeParser::parseAttributes(tinyxml2::XMLElement* element)
{
    Sprite* sprite = dynamic_cast<Sprite*>(m_node);
    LOG_ASSERT(sprite, "m_node is not a cocos2d::Sprite!");
    
    SET_XML_STR_ATTRIBUTE(element, "texture", sprite, initWithFile);
    SET_XML_STR_ATTRIBUTE(element, "frame", sprite, initWithSpriteFrameName);

    NodeParser::parseAttributes(element);
}

bool SpriteNodeParser::parseElement(tinyxml2::XMLElement* element)
{
    const string elementName = element->Name();
    // It's recommended that <resources> should be the top most tags; to prevent initialization errors.
    if (elementName == "sliced")
    {
        parseSliced(element);
        return true;
    }
    else
    {
        return NodeParser::parseElement(element);
    }
}

void SpriteNodeParser::parseSliced(tinyxml2::XMLElement* element)
{
    Sprite* sprite = dynamic_cast<Sprite*>(m_node);
    LOG_ASSERT(sprite, "m_node is not a cocos2d::Sprite!");
    
    Rect centerRect;
    element->QueryFloatAttribute("x", &centerRect.origin.x);
    element->QueryFloatAttribute("y", &centerRect.origin.y);
    element->QueryFloatAttribute("w", &centerRect.size.width);
    element->QueryFloatAttribute("h", &centerRect.size.height);

    sprite->setCenterRect(centerRect);
}

Node* SpriteNodeParser::createNode()
{
    return Sprite::create();
}