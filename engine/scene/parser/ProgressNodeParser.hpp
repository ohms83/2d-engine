#pragma once

#include "NodeParser.hpp"

class ProgressNodeParser : public NodeParser
{
public:
    ProgressNodeParser(cocos2d::Node* parent);

    const std::string getRootTag() override
    {
        return "progress";
    }

    static const std::string RADIAL_TYPE;
    static const std::string BAR_TYPE;

protected:
    bool parseAttributes(tinyxml2::XMLElement* element) override;
    bool parseElement(tinyxml2::XMLElement* element) override;

    bool parseMidPoint(tinyxml2::XMLElement* element);
    bool parseBarChangeRate(tinyxml2::XMLElement* element);

    /// Set size of the current node
    void setSize(const cocos2d::Size& size) override;
};