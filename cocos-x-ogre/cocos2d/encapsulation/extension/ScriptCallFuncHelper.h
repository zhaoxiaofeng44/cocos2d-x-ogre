#ifndef __BERYL_SCRIPT_CALLFUNC_HELPER_H__
#define __BERYL_SCRIPT_CALLFUNC_HELPER_H__


#include "cocos2d.h"

NS_CC_BEGIN


class ScriptCallFuncHelper : public Ref
{
public:    

    static ScriptCallFuncHelper *create();
	
	static ScriptCallFuncHelper *create(int id);

	void addScriptListener(int id);

	void removeScriptListener();

	void excuteScript(const Value &value);
	
	void registerScriptManager(const std::function<void(const Value &value,int id)>& func);

CC_CONSTRUCTOR_ACCESS:

    ScriptCallFuncHelper();

    virtual ~ScriptCallFuncHelper();

protected:

    int mhandlerId;
	std::function<void(const Value &value,int id)> mScriptManagerHandler;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(ScriptCallFuncHelper);
};

NS_CC_END


#endif //__BERYL_SCRIPT_CALLFUNC_HELPER_H__