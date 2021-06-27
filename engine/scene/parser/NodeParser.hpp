#pragma once

#include "cocos2d.h"
#include "external/tinyxml2/tinyxml2.h"

#define SET_XML_ATTRIBUTE(_element, _type, _name, _getter, _node, _setter) { \
    _type value; \
    if (tinyxml2::XML_NO_ERROR == _element->Query##_getter(_name, &value)) { \
        _node->_setter(value); \
    } \
}

#define SET_XML_STR_ATTRIBUTE(_element, _name, _node, _setter) { \
    const char* value = _element->Attribute(_name); \
    if (value) { \
        _node->_setter(value); \
    } \
}
#define SET_XML_INT_ATTRIBUTE(_element, _name, _node, _setter) SET_XML_ATTRIBUTE(_element, int, _name, IntAttribute, _node, _setter)
#define SET_XML_FLOAT_ATTRIBUTE(_element, _name, _node, _setter) SET_XML_ATTRIBUTE(_element, float, _name, FloatAttribute, _node, _setter)
#define SET_XML_BOOL_ATTRIBUTE(_element, _name, _node, _setter) SET_XML_ATTRIBUTE(_element, bool, _name, BoolAttribute, _node, _setter)

class NodeParser
{
public:
    NodeParser(cocos2d::Node* parent);

    bool parseFile(const std::string& file);
    virtual cocos2d::Node* parse(tinyxml2::XMLElement* element);

    virtual const std::string getRootTag()
    {
        return "node";
    }

    /**
     *  Used in @c type property of @c <position> and @c <size> tags. When data type is @c absolute
     *  the position and size parameters will be used as-is. When the type is undefined, the parameter
     *  will always be assumed this type.
     */
    static const std::string ABSOLUTE_TYPE;
    /**
     *  Used in @c type property of @c <position> and @c <size> tags. When data type is @c relative
     *  the position and size parameters are proportions of the node's parent.
     *  For example, x="0.5" means the node's X coordinate is at the center of it's parent.
     */
    static const std::string RELATIVE_TYPE;

protected:
    /// Parse node's attributes
    virtual void parseAttributes(tinyxml2::XMLElement* element);
    /// Parse child elements
    virtual void parseChildren(tinyxml2::XMLElement* element);
    /**
     * Inspect the tag name and parse the given element accordingly.
     * @return @c true , if the element's tag name can be recognised and correctly parsed; otherwise, @c false
     */
    virtual bool parseElement(tinyxml2::XMLElement* element);
    /// Parse @c position element
    virtual void parsePosition(tinyxml2::XMLElement* element);
    /// Parse @c scale element
    virtual void parseScale(tinyxml2::XMLElement* element);
    /// Parse @c size element
    virtual void parseSize(tinyxml2::XMLElement* element);
    /// Parse @c anchor element
    virtual void parseAnchor(tinyxml2::XMLElement* element);
    /// Parse @c color element
    virtual void parseColor(tinyxml2::XMLElement* element);
    /// Set position of the current node
    virtual void setPosition(const cocos2d::Vec2& pos);
    /// Set scale of the current node
    virtual void setScale(const cocos2d::Vec2& scale);
    /// Set size of the current node
    virtual void setSize(const cocos2d::Size& size);
    /// Set anchor-point of the current node
    virtual void setAnchor(const cocos2d::Vec2& anchor);
    /// Set color of the current node
    virtual void setColor(const cocos2d::Color3B& color);

    virtual cocos2d::Node* createNode();

    static bool checkIfRelative(tinyxml2::XMLElement* element);

protected:
    cocos2d::Node* m_parent = nullptr;
    cocos2d::Node* m_node = nullptr;
};