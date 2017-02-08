/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __CCBones_Animation_H__
#define __CCBones_Animation_H__

#include <map>
#include <unordered_map>

#include "3d/CCAnimation3D.h"
#include "base/ccMacros.h"
#include "base/CCRef.h"
#include "2d/CCActionInterval.h"

NS_CC_BEGIN

class Bone3D;
class Sprite3D;
/**
 * BonesAnimate3D, Animates a Sprite3D given with an Animation3D
 */
class BonesAnimate3D: public ActionInterval
{
public:
    
    /**create BonesAnimate3D using Animation.*/
    static BonesAnimate3D* create(Animation3D* animation,bool repeat = false,float transTime = 0.1f);
    
    /**
     * create BonesAnimate3D
     * @param animation used to generate BonesAnimate3D
     * @param formTime 
     * @param duration Time the BonesAnimate3D lasts
     * @return BonesAnimate3D created using animate
     */
    static BonesAnimate3D* create(Animation3D* animation,bool repeat,float fromTime, float duration);
    
    /**
     * create BonesAnimate3D by frame section, [startFrame, endFrame)
     * @param animation used to generate BonesAnimate3D
     * @param startFrame
     * @param endFrame
     * @param frameRate default is 30 per second
     * @return BonesAnimate3D created using animate
     */
    static BonesAnimate3D* createWithFrames(Animation3D* animation,bool repeat,int startFrame, int endFrame, float frameRate = 30.f);
    
    //
    // Overrides
    //
    virtual void stop() override;
    virtual void step(float dt) override;
    virtual void startWithTarget(Node *target) override;
    virtual BonesAnimate3D* reverse() const override;
    virtual BonesAnimate3D *clone() const override;
    
    virtual void update(float t) override;
    
	virtual bool isDone() const;

    /**get & set speed, negative speed means playing reverse */
    float getSpeed() const;
    void setSpeed(float speed);
    
    /**get & set blend weight, weight must positive*/
    float getWeight() const { return _weight; }
    void setWeight(float weight);

	float getAnimationLength() const { return _animationLength;  }
	void setAnimationLength(float length) { _animationLength = length;  }
  
	bool getRepeat() const { return _repeat;  }
	void setRepeat(bool repeat) { _repeat = repeat;  }

    /** animate transistion time */
    float getTransitionTime() { return _transTime; }
    void setTransitionTime(float transTime) { _transTime = transTime; }

    /**get & set play reverse, these are deprecated, use set negative speed instead*/
    CC_DEPRECATED_ATTRIBUTE bool getPlayBack() const { return _playReverse; }
    CC_DEPRECATED_ATTRIBUTE void setPlayBack(bool reverse) { _playReverse = reverse; }
    
CC_CONSTRUCTOR_ACCESS:
    
    BonesAnimate3D();
    virtual ~BonesAnimate3D();
    
    void removeFromMap();
    
protected:
    enum class BonesAnimationState
    {
        FadeIn,
        FadeOut,
        Running,
    };
    BonesAnimationState _state; //animation state
    Animation3D* _animation; //animation data

	
	bool       _repeat;
	bool       _isDone;
	float      _transTime; //transition time from one BonesAnimate3D to another

    float      _absSpeed; //playing speed
    float      _weight; //blend weight
    float      _start; //start time 0 - 1, used to generate sub BonesAnimate3D
    float      _last; //last time 0 - 1, used to generate sub BonesAnimate3D
    bool       _playReverse; // is playing reverse
   
    float      _accTransTime; // acculate transition time
    float      _lastTime;     // last t (0 - 1)
	float	   _animationLength; // then animation length(at speed 1)
    std::unordered_map<Bone3D*, Animation3D::Curve*> _boneCurves; //weak ref

    //sprite animates
    static std::unordered_map<Sprite3D*, BonesAnimate3D*> s_fadeInAnimates;
    static std::unordered_map<Sprite3D*, BonesAnimate3D*> s_fadeOutAnimates;
    static std::unordered_map<Sprite3D*, BonesAnimate3D*> s_runningAnimates;
};

NS_CC_END

#endif // __CCBonesAnimation_H__
