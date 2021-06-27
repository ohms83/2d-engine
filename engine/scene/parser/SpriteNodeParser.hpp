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
    void parseAttributes(tinyxml2::XMLElement* element) override;
    cocos2d::Node* createNode() override;
};