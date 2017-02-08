#ifndef OGRE_APP_MGR_H
#define OGRE_APP_MGR_H

#include "cocos2d.h"

namespace Ogre
{
	class Root;
	class Entity;
	class Camera;
	class Viewport;
	class RenderWindow;
	class RenderSystem;
	class BridgeSceneManager;
	class ScriptLoader;
	class MovableObject;
	class SceneManager;
}

namespace ParticleUniverse
{
	class ParticleSystem;
}

NS_CC_BEGIN

class OgreManager
{
public:  //root

	void initialize();

	void purge();

	ParticleUniverse::ParticleSystem* createParticleSystem(const std::string &typeName,const std::string &entityName);

	void destroyParticleSystem(ParticleUniverse::ParticleSystem* particleSystem);

	Ogre::Entity* createEntity(const std::string &fileName);

	void destroyEntity(Ogre::Entity* entity);

	void addResourceByName(const std::string &name, const std::string &grop = "General");

	void removeResourceByName(const std::string &name, const std::string &grop = "General");

	static OgreManager *getInstance();

	Ogre::Camera* getCamera();

	Ogre::RenderSystem* getRenderSystem();

	Ogre::SceneManager* getSceneManager();

CC_CONSTRUCTOR_ACCESS:

	OgreManager();

	virtual ~OgreManager();

protected:

	bool addResourceForManager(const std::string &, const std::string &, Ogre::ScriptLoader *);

	Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	Ogre::RenderWindow* mWindow;
	Ogre::RenderSystem *mRenderSystem;
	Ogre::BridgeSceneManager *mSceneMgr;

};

NS_CC_END

#endif//OGREAPP_H
