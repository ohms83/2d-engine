//
//  AnimationUtil.hpp
//  MiniRPG
//
//  Created by nuttachai on 17/5/20.
//

#ifndef AnimationUtil_hpp
#define AnimationUtil_hpp

#include "cocos2d.h"

#include <ios>

namespace animationUtil
{
    /**
     *  Parse and create an Animation object from specified @c inputStream .
     *  @discussion
     *  The 'animation' data format is as of the following
     *      "format":   Either one of "sprite-sheet" or "texture-list" -- the later is discourage due to performance issue
     *                  (multiple textures means more draw calls).
     *      "sheet":    [Optional] Only valid if the 'format' is 'sprite-sheet'. If assigned, the funciton will try to load
     *                  specified .plist file if it wasn't already loaded.
     *      "frames":   A list of animation sequences; for example, ["walk1.png", "walk2.png", ... "walkN.png"].
     *      "interval": Delay timer between frames in second.
     *      "repeat":   Number of time the animation should be repeated. -1 means the animation will be looped forever.
     *
     *  @return If success, an @c Animation object; otherwise @c nullptr.
     */
    cocos2d::Animation* parseAnimation(std::istream& inputStream);
    
    /**
     *  Load and create an Animation object from specified @c jsonFile . This function internally use @c parseAnimation to
     *  parse the JSON data file.
     *
     *  @return If success, an @c Animation object; otherwise @c nullptr.
     *
     *  @see animation::util::parseAnimationJson
     */
    cocos2d::Animation* createAnimation(const std::string& jsonFile);
}

#endif /* AnimationUtil_hpp */
