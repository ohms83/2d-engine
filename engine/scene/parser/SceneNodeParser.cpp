#include "SceneNodeParser.hpp"

#include "engine/debug/DebugMacros.h"
#include "engine/util/TypeCheck.h"

#include "../GameScene.hpp"

USING_NS_CC;
using namespace std;

SceneNodeParser::SceneNodeParser(GameScene* scene)
: NodeParser(scene)
{
    m_director = Director::getInstance();
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
            const char* plist = element->Attribute("plist");
            const char* texFile = element->Attribute("texture");
            if (plist)
            {
                Texture2D* texture = ( texFile ? m_director->getTextureCache()->addImage(texFile) : nullptr );
                if (texture) {
                    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist, texture);
                }
                else {
                    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist);
                }
            }
            else {
                LOG_ERROR("Error parsing <spriteFrame> REASON='plist' attribute is missing.");
            }
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