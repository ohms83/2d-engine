#pragma once

#include "cocos2d.h"

#include <map>

namespace util
{
    /// Utility functions for to cocos2d's classes
    namespace cocosutil
    {
        /// Create an auto-release object of @c _Class
        template<typename _Class>
        _Class* create()
        {
            _Class* result = new (std::nothrow) _Class();
            result->autorelease();
            return result;
        }

        /// Create and initialize an auto-release object of @c _Class
        template<typename _Class>
        _Class* createAndInit()
        {
            _Class* result = new (std::nothrow) _Class();
            if (result && result->init())
            {
                result->autorelease();
                return result;
            }
            else
            {
                delete result;
                return nullptr;
            }
        }

        /**
         *  Create a list of @c DrawNode that drawing @c target node's bounding-box and origin
         *  @param target Target node
         *  @param axisWidth Width of the debug X and Y axes in pixel; the default value is 2 pixels
         *  @param axisLength Length of the debug X and Y axes in pixel; the default value is 32 pixels
         *  @param anchorPointRadius the size of the debug anchor point in pixel; the default value is 4
         *  
         *  @remark
         *  A new stub node named "_DebugRootNode" will be added as a parent node of all debug drawers.
         *  To disable debug draw, just remove this node or call @c removDebugDrawFromNode function
         */
        void attachDebugDrawToNode(cocos2d::Node* target, int axisWidth = 2, int axisLength = 32, int anchorPointRadius = 4);
        /// Removed all the debug-drawer nodes created from @c attachDebugDrawToNode from the @c target.
        void removDebugDrawFromNode(cocos2d::Node* target);

        static const char* DEBUG_DRAW_ROOT = "_DebugNodeRoot";

        /**
         *  Parse the given string and convert it to @c cocos2d::EventKeyboard::KeyCode
         *  @param strKey A string representing @c cocos2d::EventKeyboard::KeyCode keys (case-insensity).
         *  @return @c cocos2d::EventKeyboard::KeyCode key matching the string or @c cocos2d::EventKeyboard::KEY_NONE upon error.
         *  @see @c KEYCODE_MAP
         */
        cocos2d::EventKeyboard::KeyCode parseKeyCode(const std::string& strKey);
        /// Mapping between strings and @c cocos2d::EventKeyboard::KeyCode keys
        extern const std::map<std::string, cocos2d::EventKeyboard::KeyCode> KEYCODE_MAP;
    }
}