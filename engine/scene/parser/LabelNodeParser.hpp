#pragma once

#include "NodeParser.hpp"

class LabelNodeParser : public NodeParser
{
public:
    LabelNodeParser(cocos2d::Node* parent);

    const std::string getRootTag() override
    {
        return "sprite";
    }

    static const std::string SYSTEM_FONT;
    static const std::string TRUE_TYPE_FONT;
    static const std::string BITMAP_FONT;

    static const std::string OVERFLOW_CLAMP;
    static const std::string OVERFLOW_SHRINK;
    static const std::string OVERFLOW_RESIZE;

    static const std::string ALIGN_TOP;
    static const std::string ALIGN_CENTER;
    static const std::string ALIGN_BOTTOM;
    static const std::string ALIGN_LEFT;
    static const std::string ALIGN_RIGHT;

protected:
    void parseAttributes(tinyxml2::XMLElement* element) override;
    bool parseFont(tinyxml2::XMLElement* element, cocos2d::Label* label);
    void parseOverflow(tinyxml2::XMLElement* element, cocos2d::Label* label);
    void parseVAlign(tinyxml2::XMLElement* element, cocos2d::Label* label);
    void parseHAlign(tinyxml2::XMLElement* element, cocos2d::Label* label);

    cocos2d::Node* createNode() override;
    
    void setSize(const cocos2d::Size& size) override;
};