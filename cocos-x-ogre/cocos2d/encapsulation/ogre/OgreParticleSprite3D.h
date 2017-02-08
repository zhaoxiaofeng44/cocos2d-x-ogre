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

#ifndef __CCParticleSprite3D_H__
#define __CCParticleSprite3D_H__

#include "cocos2d.h"
#include "OgreOgreSprite3D.h"
#include <ParticleUniverseCommon.h>
#include <ParticleUniverseSystemListener.h>

namespace ParticleUniverse
{
	class ParticleSystem;
}

NS_CC_BEGIN

/** particle univeise system */
class ParticleSprite3D :
	public OgreSprite3D,
	public ParticleUniverse::ParticleSystemListener
{
public:

	void showParticle();

	void stopParticle();

	void autoRemoveSelf();

	void setKeepLocal(bool isLocal);

	static ParticleSprite3D* create(const std::string &typeName);
	void setDynamicAttribute(const std::string &attributeName,float value){}
	virtual void handleParticleSystemEvent(ParticleUniverse::ParticleSystem* particleSystem, ParticleUniverse::ParticleUniverseEvent& particleUniverseEvent) override;

	virtual void updateParentTransform(const Mat4& parentTransform);

CC_CONSTRUCTOR_ACCESS:

	ParticleSprite3D();
	
	virtual ~ParticleSprite3D();

	bool initWithFile( const std::string &typeName);

	virtual void update(float delta) override;

protected:

	enum StateType
	{
		PARTICLE_NONE,	
		PARTICLE_STARTING,	
		PARTICLE_STOPPING
	};

	bool mIsRunning;

	StateType mStateType;
	bool mParticleStateDirty;

	bool mIsStartParticle;
	bool mIsAutoRemoveSelf;

	ParticleUniverse::ParticleSystem *mParticleSystem;
};


NS_CC_END
#endif // __ParticleSprite3D_H_
