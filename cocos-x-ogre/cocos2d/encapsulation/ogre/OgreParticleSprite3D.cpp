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

#include "OgreParticleSprite3D.h"

#include "OgreOgreManager.h"
#include "OgreNodeProtocol.h"

#include <ParticleUniverseSystemManager.h>

NS_CC_BEGIN

ParticleSprite3D* ParticleSprite3D::create(const std::string &typeName)
{
	auto node = new ParticleSprite3D();
	if (node && node->initWithFile(typeName))
	{
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return nullptr;
}

ParticleSprite3D::ParticleSprite3D() :
	mIsRunning(false),
	mParticleSystem(NULL),
	mIsAutoRemoveSelf(false),
	mStateType(PARTICLE_NONE),
	mParticleStateDirty(false)
{
}

ParticleSprite3D::~ParticleSprite3D()
{
	if (mParticleSystem)
	{
		this->detachObject(mParticleSystem);
		mParticleSystem->removeParticleSystemListener(this);
		OgreManager::getInstance()->destroyParticleSystem(mParticleSystem);
		mParticleSystem = NULL;
	}
}

bool ParticleSprite3D::initWithFile(const std::string &typeName)
{
	std::stringstream ss; 
	ss << this;
	std::string name = "ParticleSprite3D" + ss.str();
	mParticleSystem = OgreManager::getInstance()->createParticleSystem(typeName,name);
	if (OgreSprite3D::init() && mParticleSystem)
	{
		scheduleUpdate();

		this->attachObject(mParticleSystem);
		mParticleSystem->setUseController(false);
		mParticleSystem->setBoundsAutoUpdated(false);
		mParticleSystem->addParticleSystemListener(this);

		return true;
	}
	return false;
}

void ParticleSprite3D::showParticle()
{
	mIsStartParticle = true;
}

void ParticleSprite3D::stopParticle()
{
	mParticleSystem->stopFade();
}

void ParticleSprite3D::autoRemoveSelf()
{
	mIsAutoRemoveSelf = true;
}

void ParticleSprite3D::setKeepLocal(bool isLocal)
{
	mParticleSystem->setKeepLocal(isLocal);
}

void ParticleSprite3D::updateParentTransform(const Mat4& parentTransform)
{
	OgreSprite3D::updateParentTransform(parentTransform);
	
	if (_nodeProtocol)
	{
		mParticleSystem->setScale(Ogre::Vector3(mCosScale.x,mCosScale.y,mCosScale.z));
		mParticleSystem->setScaleVelocity(mCosScale.x);
		mParticleSystem->_update(0);
	}
}

void ParticleSprite3D::update(float delta)
{
	if(mIsStartParticle)
	{
		mIsStartParticle = false;
		mParticleSystem->start();
	}
	else
	{
		if(mIsRunning)
		{
			mParticleSystem->_update(delta);
		}

		if(mParticleStateDirty)
		{
			mParticleStateDirty = false;
			switch(mStateType)
			{
			case PARTICLE_STARTING:
				mIsRunning = true;
				break;
			case PARTICLE_STOPPING:
				mIsRunning = false;
				if(mIsAutoRemoveSelf)  //cannot write any code after remove
				{
					this->removeFromParent();
				}
				break;
			}
		}
	}	
}

void ParticleSprite3D::handleParticleSystemEvent(ParticleUniverse::ParticleSystem* particleSystem, ParticleUniverse::ParticleUniverseEvent& particleUniverseEvent)
{
	switch(particleUniverseEvent.eventType)
	{
	case ParticleUniverse::EventType::PU_EVT_SYSTEM_STARTING:
			mParticleStateDirty = true;
			mStateType = StateType::PARTICLE_STARTING;
		break;
	case ParticleUniverse::EventType::PU_EVT_SYSTEM_STOPPING:
			mParticleStateDirty = true;
			mStateType = StateType::PARTICLE_STOPPING;
		break;
	}

	/*
	ParticleUniverse::EventType::PU_EVT_SYSTEM_ATTACHING,		// Submit event when the particle system is being attached or detached.
	ParticleUniverse::EventType::PU_EVT_SYSTEM_ATTACHED,			// Submit event when the particle system is attached or detached.
	ParticleUniverse::EventType::PU_EVT_SYSTEM_PREPARING,		// Submit event when the particle system is preparing.
	ParticleUniverse::EventType::PU_EVT_SYSTEM_PREPARED,			// Submit event when the particle system is prepared.
	ParticleUniverse::EventType::PU_EVT_SYSTEM_STARTING,			// Submit event when the particle system is starting.
	ParticleUniverse::EventType::PU_EVT_SYSTEM_STARTED,			// Submit event when the particle system is started.
	ParticleUniverse::EventType::PU_EVT_SYSTEM_STOPPING,			// Submit event when the particle system is stopping.
	ParticleUniverse::EventType::PU_EVT_SYSTEM_STOPPED,			// Submit event when the particle system is stopped.
	ParticleUniverse::EventType::PU_EVT_SYSTEM_PAUSING,			// Submit event when the particle system is pausing.
	ParticleUniverse::EventType::PU_EVT_SYSTEM_PAUSED,			// Submit event when the particle system is paused.
	ParticleUniverse::EventType::PU_EVT_SYSTEM_RESUMING,			// Submit event when the particle system is resuming (after a pause).
	ParticleUniverse::EventType::PU_EVT_SYSTEM_RESUMED,			// Submit event when the particle system is resumed (after a pause).
	ParticleUniverse::EventType::PU_EVT_SYSTEM_DELETING,			// Submit event when the particle system is being deleted.
	ParticleUniverse::EventType::PU_EVT_LOD_TRANSITION,			// Submit event when the particle system switches to another technique when a LOD-level is exceeded.
	ParticleUniverse::EventType::PU_EVT_EMITTER_STARTED,			// Submit event when an emitter is started.
	ParticleUniverse::EventType::PU_EVT_EMITTER_STOPPED,			// Submit event when an emitter is stopped.
	ParticleUniverse::EventType::PU_EVT_NO_PARTICLES_LEFT,		// Submit event when all particles have been expired.*/
	
}


NS_CC_END