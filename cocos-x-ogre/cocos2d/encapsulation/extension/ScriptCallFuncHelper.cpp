#include "ScriptCallFuncHelper.h"

NS_CC_BEGIN

ScriptCallFuncHelper *ScriptCallFuncHelper::create()
{
	ScriptCallFuncHelper *ret = new (std::nothrow) ScriptCallFuncHelper();
    if (ret)
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

ScriptCallFuncHelper *ScriptCallFuncHelper::create(int id)
{
	ScriptCallFuncHelper *ret = ScriptCallFuncHelper::create();
	if(ret)
	{
		ret->addScriptListener(id);
	}
	return ret;
}

void ScriptCallFuncHelper::excuteScript(const Value &value)
{
	if(mhandlerId && mScriptManagerHandler)
	{
		mScriptManagerHandler(value,mhandlerId);
	}
}

void ScriptCallFuncHelper::addScriptListener(int id)
{
	mhandlerId = id;
}

void ScriptCallFuncHelper::removeScriptListener()
{
	mhandlerId = 0;
}

void ScriptCallFuncHelper::registerScriptManager(const std::function<void(const Value &value,int id)>& func)
{
	mScriptManagerHandler = func;
}

ScriptCallFuncHelper::ScriptCallFuncHelper()
	:mhandlerId(0)
{
}

ScriptCallFuncHelper::~ScriptCallFuncHelper()
{
	mhandlerId = 0;
}

NS_CC_END