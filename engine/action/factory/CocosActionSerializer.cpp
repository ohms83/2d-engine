//
//  CocosActionSerializer.cpp
//  MiniRPG
//
//  Created by nuttachai on 23/5/20.
//

#include "CocosActionSerializer.hpp"

USING_NS_CC;
using namespace std;

Action* DelayTimeSerializer::create(const data::Serializable& obj)
{
    const data::Serializable::Type objType = obj.getType();
    if (objType != data::Serializable::Type::MAP)
    {
        CCLOGWARN("ERROR: Unsupport serializable object (%s). DelayTimeSerializer can only unpack data::Serializable::Type::MAP.",
                  data::Serializable::getTypeString(objType).c_str());
        return nullptr;
    }
    const data::Serializable::Map& mapObj = obj.getMap(); // Converts obj to data::Serializable::Map.
    float dt = (float)(double)(mapObj.at("dt")); // Calling 'double' casting operator then cast it down to float.
    return DelayTime::create(dt);
}

Action* AnimateSerializer::create(const data::Serializable& obj)
{
    static FileUtils* fileUtil = FileUtils::getInstance();
    
    string spriteSheet = std::move(obj.getValue("spriteframe").getString());
    string spriteSheetFullPath = std::move(fileUtil->fullPathForFilename(spriteSheet));
    if(spriteSheetFullPath.empty())
    {
        CCLOGWARN("Cannot find sprite sheet %s. Possible missing file", spriteSheet.c_str());
        return nullptr;
    }
    
    SpriteFrameCache* spriteFrameCache = SpriteFrameCache::getInstance();
    spriteFrameCache->addSpriteFramesWithFile(spriteSheet);
    
    float interVal = obj.getValue("interval").getFloat();
    
    // Parsing frame list
    const data::Serializable::Array& frames = obj.getValue("frames").getArray();
    Vector<SpriteFrame*> animFrames;
    
    for(const auto& serializedFrame : frames)
    {
        const std::string& frameName = serializedFrame.getString();
        SpriteFrame* frame = spriteFrameCache->getSpriteFrameByName(frameName);
        if(frame)
        {
            animFrames.pushBack(frame);
        }
        else
        {
            CCLOGWARN("Cannot find a sprite frame by name: %s", frameName.c_str());
        }
    }
    
    if(animFrames.empty())
    {
        return nullptr;
    }
    
    Animation* animation = Animation::createWithSpriteFrames(animFrames, interVal);
    return Animate::create(animation);
}
