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
Node* SpriteNodeParser::createNode()
{
    return Sprite::create();
}