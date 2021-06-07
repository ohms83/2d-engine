//
//  AnimationUtil.cpp
//  MiniRPG
//
//  Created by nuttachai on 17/5/20.
//

#include "AnimationUtil.hpp"
#include "engine/serialize/JsonSerializer.hpp"
#include "engine/util/TypeCheck.h"

#include <fstream>

using namespace std;
using namespace serialize;
using namespace debug;

cocos2d::Animation* animation::util::parseAnimationJson(istream& input)
{
    CHECK_IF_RETURN_MSG(!input.good(), nullptr, "Parsing Error. REASON=Invalid input stream");
    
    static cocos2d::SpriteFrameCache* spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
    
    cocos2d::Animation* result = nullptr;
    
    JsonSerializer serializer;
    Serializable serializeObj = serializer.unpack(input);
    
    if (serializeObj.getType() != Serializable::Type::MAP)
    {
        string json;
        input.seekg(0);
        input >> json;
        
        stringstream ss;
        ss << "Parsing Error. REASON=Invalid JSON format"
            << " JSON=" << json;
        LOG_ERROR("%s", ss.str().c_str());
        return nullptr;
    }
    
    // All recognized keys.
    const string FORMAT_KEY = "format";
    const string INTERVAL_KEY = "interval";
    const string REPEAT_KEY = "repeat";
    const string FRAMES_KEY = "frames";
    const string SHEET_KEY = "sheet";
    
    // Check for mandatory keys
    {
        std::array<const char*, 4> mandatoryKeys = { FORMAT_KEY.c_str(), INTERVAL_KEY.c_str(), REPEAT_KEY.c_str(), FRAMES_KEY.c_str() };
        for (auto key : mandatoryKeys)
        {
            CHECK_IF_RETURN_MSG(!serializeObj.hasKey(key), nullptr, "Parsing Error. REASON=\"%s\" parameter cannot be omitted", key);
        }
    }
    
    const string SPRITE_SHEET_FORMAT = "sprite-sheet";
    const string TEXTURE_LIST_FORMAT = "texture-list";
    
    bool isUsingSpriteSheet = false;
    const string format = serializeObj[FORMAT_KEY].getString();
    
    if (format == SPRITE_SHEET_FORMAT)
    {
        isUsingSpriteSheet = true;
    }
    else if(format != TEXTURE_LIST_FORMAT)
    {
        LOG_WARNING("Unrecognizable format. 'texture-list' will be assumed. FORMAT=\"%s\"", format.c_str());
    }
    
    float interval = serializeObj[INTERVAL_KEY].getFloat();
    
    int loop = serializeObj[REPEAT_KEY].getInt();
    if (loop < 0) {
        loop = INT_MAX;
    }
    
    const Serializable::Array& frames = serializeObj[FRAMES_KEY].getArray();
    
    if (isUsingSpriteSheet)
    {
        Serializable value("");
        
        if (serializeObj.tryGet("sheet", value))
        {
            const string& sheet = value.getString();
            if (!spriteFrameCache->isSpriteFramesWithFileLoaded(sheet)) {
                spriteFrameCache->addSpriteFramesWithFile(sheet);
            }
        }
        
        cocos2d::Vector<cocos2d::SpriteFrame*> spriteFrames;
        
        for (const auto& frameName : frames)
        {
            cocos2d::SpriteFrame* spriteFrame = spriteFrameCache->getSpriteFrameByName(frameName);
            spriteFrames.pushBack(spriteFrame);
        }
        
        result = cocos2d::Animation::createWithSpriteFrames(spriteFrames, interval, loop);
    }
    else
    {
        static cocos2d::Director* director = cocos2d::Director::getInstance();
        result = cocos2d::Animation::create();
        
        for (const auto& frameName : frames)
        {
            cocos2d::Texture2D *texture = director->getTextureCache()->addImage(frameName);
            CHECK_IF_NULL_RETURN_MSG(texture, nullptr, "Parsing Error. REASON=Invalid texture file name FILENAME=%s", frameName.getString().c_str());
            
            cocos2d::Rect rect = cocos2d::Rect::ZERO;
            rect.size = texture->getContentSize();
            result->addSpriteFrameWithTexture(texture, rect);
        }
    }
    
    return result;
}

cocos2d::Animation* animation::util::createAnimationFromJson(const string& jsonFile)
{
    static cocos2d::FileUtils* fileUtils = cocos2d::FileUtils::getInstance();
    
    CHECK_IF_RETURN_MSG(!fileUtils->isFileExist(jsonFile), nullptr, "Parsing error REASON=Invalid file path FILE=%s", jsonFile.c_str());
    
    string jsonStr = fileUtils->getStringFromFile(jsonFile);
    stringstream ss;
    ss << jsonStr;
    return parseAnimationJson(ss);
}

cocos2d::Map<std::string, cocos2d::Animation*> animation::util::parseAnimationListJson(istream& input)
{
    cocos2d::Map<std::string, cocos2d::Animation*> animList = cocos2d::Map<std::string, cocos2d::Animation*>();
    
    CHECK_IF_RETURN_MSG(!input.good(), animList, "Parsing Error. REASON=Invalid input stream");
    
    JsonSerializer serializer;
    Serializable serializeObj = serializer.unpack(input);
    
    if (serializeObj.getType() != Serializable::Type::MAP)
    {
        string json;
        input.seekg(0);
        input >> json;
        
        stringstream ss;
        ss << "[animation::util::parseAnimationJson] Parsing Error. REASON=Invalid JSON format"
            << " JSON=" << json;
        LOG_ERROR("%s", ss.str().c_str());
        return animList;
    }
    
    const Serializable::Map& mapData = serializeObj.getMap();
    
    for (const auto& keyValue : mapData)
    {
        const string& name = keyValue.first;
        const string& animFilePath = keyValue.second.getString();
        cocos2d::Animation* animation = animation::util::createAnimationFromJson(animFilePath);
        
        if (animation) {
            animList.insert(name, animation);
        }
    }
    
    return animList;
}

cocos2d::Map<std::string, cocos2d::Animation*> animation::util::createAnimationListFromJson(const string& jsonFile)
{
    static cocos2d::FileUtils* fileUtils = cocos2d::FileUtils::getInstance();
    
    if (fileUtils->isFileExist(jsonFile))
    {
        string jsonStr = fileUtils->getStringFromFile(jsonFile);
        stringstream ss;
        ss << jsonStr;
        return parseAnimationListJson(ss);
    }
    
    LOG_ERROR("Parsing error REASON=Invalid file path FILE=%s", jsonFile.c_str());
    return cocos2d::Map<std::string, cocos2d::Animation*>();
}
