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

#include "ScheduleAction.h"

#include "base/CCDirector.h"
#include "2d/CCCamera.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCGLProgramCache.h"
#include "extension/ScriptCallFuncHelper.h"


NS_CC_BEGIN

//
// DelayTime
//
ScheduleAction* ScheduleAction::create(float d,ScriptCallFuncHelper *helper)
{
    ScheduleAction* action = new (std::nothrow) ScheduleAction();

    action->initWithAction(d,helper);
    action->autorelease();

    return action;
}


bool ScheduleAction::initWithAction(float d,ScriptCallFuncHelper *helper)
{
    CCASSERT(helper != nullptr, "");
    CCASSERT(helper != mScriptHelper, "");

    if (ActionInterval::initWithDuration(d))
    {
        // Don't leak if action is reused
        CC_SAFE_RELEASE(mScriptHelper);

        mScriptHelper = helper;
        mScriptHelper->retain();

        return true;
    }

    return false;
}

ScheduleAction* ScheduleAction::clone() const
{
	// no copy constructor
	auto a = new (std::nothrow) ScheduleAction();
    a->initWithAction(_duration,mScriptHelper);
	a->autorelease();
	return a;
}

void ScheduleAction::update(float time)
{
	if(mScriptHelper)
	{
		mScriptHelper->excuteScript(Value(time));
	}
}

ScheduleAction* ScheduleAction::reverse() const
{
    return ScheduleAction::create(_duration,mScriptHelper);
}

ScheduleAction::ScheduleAction()
	:mScriptHelper(nullptr)
{}

ScheduleAction::~ScheduleAction()
{
	CC_SAFE_RELEASE_NULL(mScriptHelper);
}

NS_CC_END