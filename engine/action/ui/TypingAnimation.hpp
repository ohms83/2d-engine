//
//  TextTypingAnimation.hpp
//  MiniRPG
//
//  Created by nuttachai on 1/5/20.
//

#ifndef TypingAnimation_hpp
#define TypingAnimation_hpp

#include "cocos2d.h"

#include <functional>

/** An action class that imitate message typing animation where characters appearing
 *  on the screen one-by-one at a fixed interval.
 *  @warning This action only works with cocos2d::Label and its child classes. Running
 *  this action on other type of cocos2d::Node might cause unpredictable runtime errors.
 */
class TypingAnimation : public cocos2d::FiniteTimeAction
{
public:
    /** A call back that will be called for every character printed.
     *  This can be used for playing sound effect, etc.
     *  @param currentText The text that is being printed.
     */
    typedef std::function<void(const std::string&)> TypingCallback;
    
    /** Default constructure.
     *  @warning TypingAnimation are not auto-release object by default.
     *  Please use TypingAnimation::create* functions if you are not planing to
     *  manually manage allocated memory.
     */
    TypingAnimation();
    ~TypingAnimation();
    
    /** Create an auto-release object of TypingAnimation action.
     *  This is the preferable way to create TypingAnimation object.
     *  @param text Text to be animated.
     *  @param speed Typing speed in character/second
     */
    static TypingAnimation* create(const std::string& text, int speed);
    
    /** Create an auto-release object of TypingAnimation action.
     *  @param text Text to be animated.
     *  @param speed Typing speed in character/second
     *  @param callback A call back that will be called for every character printed.
     */
    static TypingAnimation* createWithCallback(const std::string& text, int speed, TypingCallback callback);
    
    /** Returns a clone of action.
     *
     * @return A clone action.
     */
    cocos2d::FiniteTimeAction* clone() const override;

    /** Returns a new action that performs the exact reverse of the action.
     *
     * @return A new action that performs the exact reverse of the action.
     * @warning This function is not implmented and will cause runtime errors.
     */
    cocos2d::FiniteTimeAction* reverse() const override;

    /** Called before the action start. It will also set the target.
     *
     * @param target A certain target.
     */
    void startWithTarget(cocos2d::Node* target) override;
    
    bool isDone() const override;
    
    void update(float time) override;
    
    /** Set typing speed.
     *  @note Action duration will be re-calculated upon calling this function.
     */
    void setTypingSpeed(int speed);
    
    /** Set text to be animated.
     *  @note Action duration will be re-calculated upon calling this function.
     */
    void setText(const std::string& text);
    
    void setCallback(TypingCallback callback)
    {
        _callback = callback;
    }
    
    static const int DEFAULT_SPEED;
    
protected:
    cocos2d::Label* _targetLabel = nullptr;
    int _speed = 0;
    /** Normalized update interval between characters.
     *  _normalizedInterval = 1 / (_speed * _maxCharacterIndex)
     */
    float _normalizedInterval = 1;
    size_t _characterIndex = 0;
    size_t _maxCharacterIndex = 0;
    //! Target text in utf32 encoding
    std::u32string _targetTextUtf32;
    std::string _animateText;
    TypingCallback _callback = nullptr;
    bool _isReverse = false;
    
    size_t getStartCharIndex() const;
    size_t getCurrentCharIndex(float time) const;
    std::string getStartString() const;
    
private:
    void computeDuration();
};
#endif /* TypingAnimation_hpp */
