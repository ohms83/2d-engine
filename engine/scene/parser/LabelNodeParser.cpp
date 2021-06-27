#include "LabelNodeParser.hpp"

#include "engine/debug/DebugMacros.h"
#include "engine/util/TypeCheck.h"

USING_NS_CC;
using namespace std;

const string LabelNodeParser::SYSTEM_FONT = "system";
const string LabelNodeParser::TRUE_TYPE_FONT = "ttf";
const string LabelNodeParser::BITMAP_FONT = "bitmap";
const string LabelNodeParser::OVERFLOW_CLAMP = "clamp";
const string LabelNodeParser::OVERFLOW_SHRINK = "shrink";
const string LabelNodeParser::OVERFLOW_RESIZE = "resize";

const string LabelNodeParser::ALIGN_TOP = "top";
const string LabelNodeParser::ALIGN_CENTER = "center";
const string LabelNodeParser::ALIGN_BOTTOM = "bottom";
const string LabelNodeParser::ALIGN_LEFT = "left";
const string LabelNodeParser::ALIGN_RIGHT = "right";

LabelNodeParser::LabelNodeParser(Node* parent)
: NodeParser(parent)
{

}
void LabelNodeParser::parseAttributes(tinyxml2::XMLElement* element)
{
    Label* label = dynamic_cast<Label*>(m_node);
    LOG_ASSERT(label, "m_node is not a cocos2d::Label!");

    if (!parseFont(element, label)) {
        return;
    }
    
    SET_XML_STR_ATTRIBUTE(element, "text", label, setString);

    parseOverflow(element, label);
    parseVAlign(element, label);
    parseHAlign(element, label);

    NodeParser::parseAttributes(element);
}
bool LabelNodeParser::parseFont(tinyxml2::XMLElement* element, cocos2d::Label* label)
{
    bool error = false;

    const char* fontName = element->Attribute("font");
    if (!fontName) {
        LOG_WARNING("Error parsing <label> REASON='font' attribute is missing!");
        error = true;
    }
    const char* fontType = element->Attribute("type");
    if (!fontType) {
        LOG_WARNING("Error parsing <label> REASON='type' attribute is missing!");
        error = true;
    }
    int fontSize = 0;
    if (tinyxml2::XML_NO_ERROR != element->QueryIntAttribute("fontSize", &fontSize)) {
        LOG_WARNING("Error parsing <label> REASON='fontSize' attribute is missing!");
        error = true;
    }

    if (error) {
        return false;
    }

    const size_t fontNameLen = strlen(fontName);
    const size_t fontTypeLen = strlen(fontType);

    if (strncmp(fontType, SYSTEM_FONT.c_str(), fontTypeLen) == 0)
    {
        label->setSystemFontName(fontName);
        label->setSystemFontSize(fontSize);
    }
    else if (strncmp(fontType, TRUE_TYPE_FONT.c_str(), fontTypeLen) == 0)
    {
        // For TTF, fontName should be a path to the font file; eg., "font/Arial.ttf".
        label->initWithTTF("", fontName, fontSize );
    }
    else if (strncmp(fontType, BITMAP_FONT.c_str(), fontTypeLen) == 0)
    {
        // TODO:
    }
    else
    {
        LOG_WARNING("Unknown font type TYPE=%s", fontType);
        return false;
    }

    return true;
}
void LabelNodeParser::parseOverflow(tinyxml2::XMLElement* element, cocos2d::Label* label)
{
    const char* overflow = element->Attribute("overflow");
    if (overflow)
    {
        const size_t overflowLen = strlen(overflow);
        if (strncmp(overflow, OVERFLOW_CLAMP.c_str(), overflowLen) == 0)
        {
            label->setOverflow(Label::Overflow::CLAMP);
        }
        else if (strncmp(overflow, OVERFLOW_SHRINK.c_str(), overflowLen) == 0)
        {
            label->setOverflow(Label::Overflow::SHRINK);
        }
        else if (strncmp(overflow, OVERFLOW_RESIZE.c_str(), overflowLen) == 0)
        {
            label->setOverflow(Label::Overflow::RESIZE_HEIGHT);
        }
        else
        {
            LOG_WARNING("Unknown overflow TYPE=%s", overflow);
        }
    }
}
void LabelNodeParser::parseVAlign(tinyxml2::XMLElement* element, cocos2d::Label* label)
{
    const char* vAlign = element->Attribute("valign");
    if (vAlign)
    {
        const size_t vAlignLen = strlen(vAlign);
        if (strncmp(vAlign, ALIGN_TOP.c_str(), vAlignLen) == 0)
        {
            label->setVerticalAlignment(TextVAlignment::TOP);
        }
        else if (strncmp(vAlign, ALIGN_CENTER.c_str(), vAlignLen) == 0)
        {
            label->setVerticalAlignment(TextVAlignment::CENTER);
        }
        else if (strncmp(vAlign, ALIGN_BOTTOM.c_str(), vAlignLen) == 0)
        {
            label->setVerticalAlignment(TextVAlignment::BOTTOM);
        }
        else
        {
            LOG_WARNING("Unknown valign TYPE=%s", vAlign);
        }
    }
}
void LabelNodeParser::parseHAlign(tinyxml2::XMLElement* element, cocos2d::Label* label)
{
    const char* hAlign = element->Attribute("halign");
    if (hAlign)
    {
        const size_t hAlignLen = strlen(hAlign);
        if (strncmp(hAlign, ALIGN_LEFT.c_str(), hAlignLen) == 0)
        {
            label->setHorizontalAlignment(TextHAlignment::LEFT);
        }
        else if (strncmp(hAlign, ALIGN_CENTER.c_str(), hAlignLen) == 0)
        {
            label->setHorizontalAlignment(TextHAlignment::CENTER);
        }
        else if (strncmp(hAlign, ALIGN_RIGHT.c_str(), hAlignLen) == 0)
        {
            label->setHorizontalAlignment(TextHAlignment::RIGHT);
        }
        else
        {
            LOG_WARNING("Unknown halign TYPE=%s", hAlign);
        }
    }
}
Node* LabelNodeParser::createNode()
{
    return Label::create();
}

void LabelNodeParser::setSize(const cocos2d::Size& size)
{
    Label* label = dynamic_cast<Label*>(m_node);
    LOG_ASSERT(label, "m_node is not a cocos2d::Label!");
    label->setDimensions(size.width, size.height);
}