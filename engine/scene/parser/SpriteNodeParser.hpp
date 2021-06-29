#pragma once

#include "NodeParser.hpp"

class SpriteNodeParser : public NodeParser
{
public:
    SpriteNodeParser(cocos2d::Node* parent);

    const std::string getRootTag() override
    {
        return "sprite";
    }

protected:
    bool parseAttributes(tinyxml2::XMLElement* element) override;
    bool parseElement(tinyxml2::XMLElement* element) override;

    bool parseSliced(tinyxml2::XMLElement* element);
};