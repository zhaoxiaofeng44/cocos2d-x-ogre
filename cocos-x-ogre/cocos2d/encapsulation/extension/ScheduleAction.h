

#ifndef __BERYL_ScheduleAction_H__
#define __BERYL_ScheduleAction_H__

#include "3d/CCAnimation3D.h"
#include "base/ccMacros.h"
#include "base/CCRef.h"
#include "2d/CCActionInterval.h"


NS_CC_BEGIN

class ScriptCallFuncHelper;

/**
 * Inherit from Sprite, achieve BillBoard.
 */
class ScheduleAction : public ActionInterval
{
public:

	/** creates the action */
    static ScheduleAction* create(float d,ScriptCallFuncHelper *helper);

	bool initWithAction(float d,ScriptCallFuncHelper *helper);

    //
    // Overrides
    //
    virtual void update(float time) override;
    virtual ScheduleAction* reverse() const override;
    virtual ScheduleAction* clone() const override;

CC_CONSTRUCTOR_ACCESS:

	ScheduleAction();

	virtual ~ScheduleAction();

protected:

	ScriptCallFuncHelper *mScriptHelper;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(ScheduleAction);
};

NS_CC_END


#endif // __BERYL_ScheduleAction_H__
