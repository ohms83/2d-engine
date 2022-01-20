#include "CocosUtil.hpp"

#include "util/TypeCheck.h"
#include "util/StringUtil.hpp"

USING_NS_CC;

using namespace std;
using namespace util;

const map<string, EventKeyboard::KeyCode> cocosutil::KEYCODE_MAP =
{
    { "none", EventKeyboard::KeyCode::KEY_NONE },
    { "pause", EventKeyboard::KeyCode::KEY_PAUSE },
    { "scroll_lock", EventKeyboard::KeyCode::KEY_SCROLL_LOCK },
    { "print", EventKeyboard::KeyCode::KEY_PRINT },
    { "sysreq", EventKeyboard::KeyCode::KEY_SYSREQ },
    { "break", EventKeyboard::KeyCode::KEY_BREAK },
    { "escape", EventKeyboard::KeyCode::KEY_ESCAPE },
    { "esc", EventKeyboard::KeyCode::KEY_ESCAPE },
    { "back", EventKeyboard::KeyCode::KEY_BACK },
    { "backspace", EventKeyboard::KeyCode::KEY_BACKSPACE },
    { "tab", EventKeyboard::KeyCode::KEY_TAB },
    { "back_tab", EventKeyboard::KeyCode::KEY_BACK_TAB },
    { "return", EventKeyboard::KeyCode::KEY_RETURN },
    { "caps", EventKeyboard::KeyCode::KEY_CAPS_LOCK },
    { "shift", EventKeyboard::KeyCode::KEY_SHIFT },
    { "left_shift", EventKeyboard::KeyCode::KEY_LEFT_SHIFT },
    { "right_shift", EventKeyboard::KeyCode::KEY_RIGHT_SHIFT },
    { "crtl", EventKeyboard::KeyCode::KEY_CTRL },
    { "left_crtl", EventKeyboard::KeyCode::KEY_CTRL },
    { "right_crtl", EventKeyboard::KeyCode::KEY_RIGHT_CTRL },
    { "alt", EventKeyboard::KeyCode::KEY_ALT },
    { "left_alt", EventKeyboard::KeyCode::KEY_LEFT_ALT },
    { "right_alt", EventKeyboard::KeyCode::KEY_RIGHT_ALT },
    { "menu", EventKeyboard::KeyCode::KEY_MENU },
    { "hyper", EventKeyboard::KeyCode::KEY_HYPER },
    { "insert", EventKeyboard::KeyCode::KEY_INSERT },
    { "ins", EventKeyboard::KeyCode::KEY_INSERT },
    { "home", EventKeyboard::KeyCode::KEY_HOME },
    { "page_up", EventKeyboard::KeyCode::KEY_PG_UP },
    { "delete", EventKeyboard::KeyCode::KEY_DELETE },
    { "del", EventKeyboard::KeyCode::KEY_DELETE },
    { "end", EventKeyboard::KeyCode::KEY_END },
    { "page_down", EventKeyboard::KeyCode::KEY_PG_DOWN },
    { "left_arrow", EventKeyboard::KeyCode::KEY_LEFT_ARROW },
    { "left", EventKeyboard::KeyCode::KEY_LEFT_ARROW },
    { "right_arrow", EventKeyboard::KeyCode::KEY_RIGHT_ARROW },
    { "right", EventKeyboard::KeyCode::KEY_RIGHT_ARROW },
    { "up_arrow", EventKeyboard::KeyCode::KEY_UP_ARROW },
    { "up", EventKeyboard::KeyCode::KEY_UP_ARROW },
    { "down_arrow", EventKeyboard::KeyCode::KEY_DOWN_ARROW },
    { "down", EventKeyboard::KeyCode::KEY_DOWN_ARROW },
    { "num_lock", EventKeyboard::KeyCode::KEY_NUM_LOCK },
    { "keypad_plus", EventKeyboard::KeyCode::KEY_KP_PLUS },
    { "keypad_minus", EventKeyboard::KeyCode::KEY_KP_MINUS },
    { "keypad_mul", EventKeyboard::KeyCode::KEY_KP_MULTIPLY },
    { "keypad_div", EventKeyboard::KeyCode::KEY_KP_DIVIDE },
    { "keypad_enter", EventKeyboard::KeyCode::KEY_KP_ENTER },
    { "keypad_home", EventKeyboard::KeyCode::KEY_KP_HOME },
    { "keypad_up", EventKeyboard::KeyCode::KEY_KP_UP },
    { "keypad_paeg_up", EventKeyboard::KeyCode::KEY_KP_PG_UP },
    { "keypad_left", EventKeyboard::KeyCode::KEY_KP_LEFT },
    { "keypad_five", EventKeyboard::KeyCode::KEY_KP_FIVE },
    { "keypad_five", EventKeyboard::KeyCode::KEY_KP_FIVE },
    { "keypad_right", EventKeyboard::KeyCode::KEY_KP_RIGHT },
    { "keypad_end", EventKeyboard::KeyCode::KEY_KP_END },
    { "keypad_down", EventKeyboard::KeyCode::KEY_KP_DOWN },
    { "keypad_page_down", EventKeyboard::KeyCode::KEY_KP_PG_DOWN },
    { "keypad_insert", EventKeyboard::KeyCode::KEY_KP_INSERT },
    { "keypad_delte", EventKeyboard::KeyCode::KEY_KP_DELETE },
    { "f1", EventKeyboard::KeyCode::KEY_F1 },
    { "f2", EventKeyboard::KeyCode::KEY_F2 },
    { "f3", EventKeyboard::KeyCode::KEY_F3 },
    { "f4", EventKeyboard::KeyCode::KEY_F4 },
    { "f5", EventKeyboard::KeyCode::KEY_F5 },
    { "f6", EventKeyboard::KeyCode::KEY_F6 },
    { "f7", EventKeyboard::KeyCode::KEY_F7 },
    { "f8", EventKeyboard::KeyCode::KEY_F8 },
    { "f9", EventKeyboard::KeyCode::KEY_F9 },
    { "f10", EventKeyboard::KeyCode::KEY_F10 },
    { "f11", EventKeyboard::KeyCode::KEY_F11 },
    { "f12", EventKeyboard::KeyCode::KEY_F12 },
    { "space", EventKeyboard::KeyCode::KEY_SPACE },
    { "!", EventKeyboard::KeyCode::KEY_EXCLAM },
    { "\"", EventKeyboard::KeyCode::KEY_QUOTE },
    { "#", EventKeyboard::KeyCode::KEY_NUMBER },
    { "$", EventKeyboard::KeyCode::KEY_DOLLAR },
    { "%", EventKeyboard::KeyCode::KEY_PERCENT },
    { "^", EventKeyboard::KeyCode::KEY_CIRCUMFLEX },
    { "&", EventKeyboard::KeyCode::KEY_AMPERSAND },
    { "\'", EventKeyboard::KeyCode::KEY_APOSTROPHE },
    { "(", EventKeyboard::KeyCode::KEY_LEFT_PARENTHESIS },
    { ")", EventKeyboard::KeyCode::KEY_RIGHT_PARENTHESIS },
    { "*", EventKeyboard::KeyCode::KEY_ASTERISK },
    { "+", EventKeyboard::KeyCode::KEY_PLUS },
    { ",", EventKeyboard::KeyCode::KEY_COMMA },
    { "-", EventKeyboard::KeyCode::KEY_MINUS },
    { ".", EventKeyboard::KeyCode::KEY_PERIOD },
    { "/", EventKeyboard::KeyCode::KEY_SLASH },
    { "0", EventKeyboard::KeyCode::KEY_0 },
    { "1", EventKeyboard::KeyCode::KEY_1 },
    { "2", EventKeyboard::KeyCode::KEY_2 },
    { "3", EventKeyboard::KeyCode::KEY_3 },
    { "4", EventKeyboard::KeyCode::KEY_4 },
    { "5", EventKeyboard::KeyCode::KEY_5 },
    { "6", EventKeyboard::KeyCode::KEY_6 },
    { "7", EventKeyboard::KeyCode::KEY_7 },
    { "8", EventKeyboard::KeyCode::KEY_8 },
    { "9", EventKeyboard::KeyCode::KEY_9 },
    { ":", EventKeyboard::KeyCode::KEY_COLON },
    { ";", EventKeyboard::KeyCode::KEY_SEMICOLON },
    { "<", EventKeyboard::KeyCode::KEY_LESS_THAN },
    { "=", EventKeyboard::KeyCode::KEY_EQUAL },
    { ">", EventKeyboard::KeyCode::KEY_GREATER_THAN },
    { "?", EventKeyboard::KeyCode::KEY_QUESTION },
    { "@", EventKeyboard::KeyCode::KEY_AT },
    { "A", EventKeyboard::KeyCode::KEY_CAPITAL_A },
    { "B", EventKeyboard::KeyCode::KEY_CAPITAL_B },
    { "C", EventKeyboard::KeyCode::KEY_CAPITAL_C },
    { "D", EventKeyboard::KeyCode::KEY_CAPITAL_D },
    { "E", EventKeyboard::KeyCode::KEY_CAPITAL_E },
    { "F", EventKeyboard::KeyCode::KEY_CAPITAL_F },
    { "G", EventKeyboard::KeyCode::KEY_CAPITAL_G },
    { "H", EventKeyboard::KeyCode::KEY_CAPITAL_H },
    { "I", EventKeyboard::KeyCode::KEY_CAPITAL_I },
    { "J", EventKeyboard::KeyCode::KEY_CAPITAL_J },
    { "K", EventKeyboard::KeyCode::KEY_CAPITAL_K },
    { "L", EventKeyboard::KeyCode::KEY_CAPITAL_L },
    { "M", EventKeyboard::KeyCode::KEY_CAPITAL_M },
    { "N", EventKeyboard::KeyCode::KEY_CAPITAL_N },
    { "O", EventKeyboard::KeyCode::KEY_CAPITAL_O },
    { "P", EventKeyboard::KeyCode::KEY_CAPITAL_P },
    { "Q", EventKeyboard::KeyCode::KEY_CAPITAL_Q },
    { "R", EventKeyboard::KeyCode::KEY_CAPITAL_R },
    { "S", EventKeyboard::KeyCode::KEY_CAPITAL_S },
    { "T", EventKeyboard::KeyCode::KEY_CAPITAL_T },
    { "U", EventKeyboard::KeyCode::KEY_CAPITAL_U },
    { "V", EventKeyboard::KeyCode::KEY_CAPITAL_V },
    { "W", EventKeyboard::KeyCode::KEY_CAPITAL_W },
    { "X", EventKeyboard::KeyCode::KEY_CAPITAL_X },
    { "Y", EventKeyboard::KeyCode::KEY_CAPITAL_Y },
    { "Z", EventKeyboard::KeyCode::KEY_CAPITAL_Z },
    { "[", EventKeyboard::KeyCode::KEY_LEFT_BRACKET },
    { "\\", EventKeyboard::KeyCode::KEY_BACK_SLASH },
    { "]", EventKeyboard::KeyCode::KEY_RIGHT_BRACKET },
    { "_", EventKeyboard::KeyCode::KEY_UNDERSCORE },
    { "`", EventKeyboard::KeyCode::KEY_GRAVE },
    { "a", EventKeyboard::KeyCode::KEY_A },
    { "b", EventKeyboard::KeyCode::KEY_B },
    { "c", EventKeyboard::KeyCode::KEY_C },
    { "d", EventKeyboard::KeyCode::KEY_D },
    { "e", EventKeyboard::KeyCode::KEY_E },
    { "f", EventKeyboard::KeyCode::KEY_F },
    { "g", EventKeyboard::KeyCode::KEY_G },
    { "h", EventKeyboard::KeyCode::KEY_H },
    { "i", EventKeyboard::KeyCode::KEY_I },
    { "j", EventKeyboard::KeyCode::KEY_J },
    { "k", EventKeyboard::KeyCode::KEY_K },
    { "l", EventKeyboard::KeyCode::KEY_L },
    { "m", EventKeyboard::KeyCode::KEY_M },
    { "n", EventKeyboard::KeyCode::KEY_N },
    { "o", EventKeyboard::KeyCode::KEY_O },
    { "p", EventKeyboard::KeyCode::KEY_P },
    { "q", EventKeyboard::KeyCode::KEY_Q },
    { "r", EventKeyboard::KeyCode::KEY_R },
    { "s", EventKeyboard::KeyCode::KEY_S },
    { "t", EventKeyboard::KeyCode::KEY_T },
    { "u", EventKeyboard::KeyCode::KEY_U },
    { "v", EventKeyboard::KeyCode::KEY_V },
    { "w", EventKeyboard::KeyCode::KEY_W },
    { "x", EventKeyboard::KeyCode::KEY_X },
    { "y", EventKeyboard::KeyCode::KEY_Y },
    { "z", EventKeyboard::KeyCode::KEY_Z },
    { "{", EventKeyboard::KeyCode::KEY_LEFT_BRACE },
    { "|", EventKeyboard::KeyCode::KEY_BAR },
    { "}", EventKeyboard::KeyCode::KEY_RIGHT_BRACE },
    { "~", EventKeyboard::KeyCode::KEY_TILDE },
    { "€", EventKeyboard::KeyCode::KEY_EURO },
    { "£", EventKeyboard::KeyCode::KEY_POUND },
    { "¥", EventKeyboard::KeyCode::KEY_YEN },
    { "·", EventKeyboard::KeyCode::KEY_MIDDLE_DOT },
    { "search", EventKeyboard::KeyCode::KEY_SEARCH },
    { "dpad_left", EventKeyboard::KeyCode::KEY_DPAD_LEFT },
    { "dpad_right", EventKeyboard::KeyCode::KEY_DPAD_RIGHT },
    { "dpad_up", EventKeyboard::KeyCode::KEY_DPAD_UP },
    { "dpad_down", EventKeyboard::KeyCode::KEY_DPAD_DOWN },
    { "dpad_center", EventKeyboard::KeyCode::KEY_DPAD_CENTER },
    { "enter", EventKeyboard::KeyCode::KEY_ENTER },
    { "play", EventKeyboard::KeyCode::KEY_PLAY },
};

void cocosutil::attachDebugDrawToNode(Node* target, int axisWidth, int axisLength, int anchorPointRadius)
{
    CHECK_IF_NULL(target);

    Node* debugRoot = Node::create();
    debugRoot->setName(DEBUG_DRAW_ROOT);
    target->addChild(debugRoot);

    const Vec2& anchorPoint = target->getAnchorPointInPoints();
    float halfAxisWidth = axisWidth * 0.5f;

    // Drawing local X axis
    {
        DrawNode* debugNode = DrawNode::create();
        debugNode->drawSolidRect(anchorPoint + Vec2(0, halfAxisWidth), anchorPoint + Vec2(axisLength, -halfAxisWidth), Color4F::BLUE);
        
        debugRoot->addChild(debugNode, 100);
    }
    // Drawing local Y axis
    {
        DrawNode* debugNode = DrawNode::create();
        debugNode->drawSolidRect(anchorPoint + Vec2(-halfAxisWidth, 0), anchorPoint + Vec2(halfAxisWidth, axisLength), Color4F::RED);
        debugRoot->addChild(debugNode, 100);
    }
    // Drawing anchor point
    {
        DrawNode* debugNode = DrawNode::create();
        debugNode->drawDot(anchorPoint, anchorPointRadius, Color4F::GREEN);
        debugRoot->addChild(debugNode, 100);
    }
    // Draw content rect
    {
        DrawNode* debugNode = DrawNode::create();

        const Size& size = target->getContentSize();
        Vec2 recOrg = Vec2(0, 0);
        Vec2 recDir = Vec2(recOrg.x + size.width, recOrg.y + size.height);
        
        debugNode->drawRect(recOrg, recDir, Color4F::YELLOW);   
        debugRoot->addChild(debugNode, 100);
    }
}

void cocosutil::removDebugDrawFromNode(Node* target)
{
    CHECK_IF_NULL(target);
    target->removeChildByName(DEBUG_DRAW_ROOT);
}

EventKeyboard::KeyCode cocosutil::parseKeyCode(const string& strKey)
{
    auto itr = KEYCODE_MAP.find(strKey);
    return (itr == KEYCODE_MAP.end() ? EventKeyboard::KeyCode::KEY_NONE : itr->second);
}

Vec2 cocosutil::inverseY(const Vec2& pos, const cocos2d::Size& parentSize)
{
    auto visibleSize = ( parentSize.equals(cocos2d::Size::ZERO) ? Director::getInstance()->getVisibleSize() : parentSize );
    Vec2 outPos(pos.x, 0);
    outPos.y = visibleSize.height - pos.y;
    return outPos;
}