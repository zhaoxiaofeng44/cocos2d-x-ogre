#include "OgreOgreManager.h"

#include <Ogre.h>

#include <OgreFontManager.h>
#include <OgreParticleFXPlugin.h>
#include <ParticleUniversePlugin.h>
#include <ParticleUniverseSystemManager.h>

#include "components/OgreBridgeImageCodec.h"
#include "components/OgreBridgeSceneManager.h"
#include "components/OgreBridgeFileSystemArchive.h"
#include "components/OgreBridgeBufferManager.h"
#include "components/OgreBridgeTextureManager.h"
#include "components/OgreBridgeRenderSystem.h"
#include "components/OgreBridgeRenderSystemPlugin.h"
#include "components/OgreBridgeWindow.h"
#include "cocos2d.h"


NS_CC_BEGIN
//-----------------------------------------------------------------------
static OgreManager *s_BridgeRootManage = NULL;
//-----------------------------------------------------------------------

OgreManager::OgreManager():
	mRoot(NULL),
	mCamera(NULL),
	mWindow(NULL),
	mSceneMgr(NULL)
{}

OgreManager::~OgreManager()
{

}

OgreManager *OgreManager::getInstance()
{
	if (!s_BridgeRootManage)
	{
		s_BridgeRootManage = new OgreManager();
	}		
	return s_BridgeRootManage;
}

void OgreManager::initialize()
{
	if(!mRoot)
	{
		auto director = cocos2d::Director::getInstance();
		
		auto origin = director->getVisibleOrigin();
		auto visibleSize = director->getVisibleSize();

		// 创建Root，在调用OGRE任何功能之前必须已经创建了Root
		mRoot = new Ogre::Root();

		//mRoot->installPlugin(new Ogre::GLES2Plugin());
		mRoot->installPlugin(new Ogre::ParticleFXPlugin());
		mRoot->installPlugin(new Ogre::BridgeRenderSystemPlugin());
		mRoot->installPlugin(new ParticleUniverse::ParticleUniversePlugin());

		Ogre::BridgeImageCodec::startup();
		mRoot->addSceneManagerFactory(new Ogre::BridgeSceneManagerFactory());
		Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::BridgeFileSystemArchiveFactory());

		mRoot->initialise();
		// 设定 RenderSystem
	    mRenderSystem = mRoot->getRenderSystemByName("Bridge Rendering Subsystem");

		mRoot->setRenderSystem(mRenderSystem);
		/*mRoot->initialise(false);*/

		mWindow = OGRE_NEW Ogre::BridgeWindow();
		mSceneMgr = static_cast<Ogre::BridgeSceneManager*>(mRoot->createSceneManager(Ogre::ST_GENERIC, Ogre::BridgeSceneManagerFactory::FACTORY_TYPE_NAME));
	
		//mSceneMgr->setViewport();
		mCamera = mSceneMgr->createCamera("PlayerCam");
		mCamera->lookAt(Ogre::Vector3(0,0,1));
		mCamera->roll(Ogre::Degree(180));
		//mCamera->yaw(Ogre::Degree(180));
		mCamera->pitch(Ogre::Degree(180));
  
		mViewport = OGRE_NEW Ogre::Viewport(mCamera, mWindow, 0, 0, 1280, 800, 0);
		mViewport->setCamera(mCamera);
		mViewport->setClearEveryFrame(false);

		/*auto data = new Ogre::RenderTarget();
		mViewport = OGRE_NEW Ogre::Viewport(mCamera, new Ogre::RenderTarget(), 0, 0, 1280, 800, 0);
		mCamera->_notifyViewport(mViewport);
	
		mViewport->setClearEveryFrame(false);
		mViewport->setBackgroundColour(Ogre::ColourValue(255, 255, 255));
	
	
		//设置屏幕的长宽比（视口的宽度和高度比，目前的宽屏电脑）
		mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
	*/
		mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
		mSceneMgr->initSceneManager(mViewport, mCamera, cocos2d::GLProgramCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
	
		Ogre::ResourceGroupManager::getSingleton()
			.addResourceLocation("LocationGeneral", "BridgeFileSystem");
	}
}


Ogre::Camera* OgreManager::getCamera(){ return mCamera;}

Ogre::RenderSystem* OgreManager::getRenderSystem(){ return mRenderSystem;}

Ogre::SceneManager* OgreManager::getSceneManager(){ return mSceneMgr;}


void OgreManager::purge()
{
	if(mRoot)
	{
		delete mRoot;
		mRoot = NULL;
	}
}


ParticleUniverse::ParticleSystem* OgreManager::createParticleSystem(const std::string &typeName,const std::string &entityName)
{
	ParticleUniverse::ParticleSystem * ret = NULL;
	if (mSceneMgr)
	{
		ParticleUniverse::ParticleSystemManager*pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		if (pManager)
		{
			ret = pManager->createParticleSystem(entityName, typeName, mSceneMgr);
		}
	}
	return ret;
}

void OgreManager::destroyParticleSystem(ParticleUniverse::ParticleSystem* particleSystem)
{
	ParticleUniverse::ParticleSystem * ret = NULL;
	if (mSceneMgr)
	{
		ParticleUniverse::ParticleSystemManager *pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		if (pManager)
		{
			pManager->destroyParticleSystem(particleSystem, mSceneMgr);
		}
	}
}

Ogre::Entity* OgreManager::createEntity(const std::string &fileName)
{
	Ogre::Entity * ret = NULL;
	if (mSceneMgr)
	{
		ret = mSceneMgr->createEntity(fileName);
	}
	return ret;
}

void OgreManager::destroyEntity(Ogre::Entity* entity)
{
	if (mSceneMgr)
	{
		mSceneMgr->destroyEntity(entity);
	}
}

void OgreManager::addResourceByName(const std::string &nameStr, const std::string &gropStr)
{
	bool ret = false;
	if (false == ret)
	{
		ret = addResourceForManager(nameStr, gropStr, Ogre::ScriptCompilerManager::getSingletonPtr());
	}

	if (false == ret)
	{
		ret = addResourceForManager(nameStr, gropStr, Ogre::FontManager::getSingletonPtr());
	}

	if (false == ret)
	{
		ret = addResourceForManager(nameStr, gropStr, Ogre::OverlayManager::getSingletonPtr());
	}
}

void OgreManager::removeResourceByName(const std::string &name, const Ogre::String &grop)
{
	Ogre::ResourceGroupManager::getSingleton().deleteResource(name,grop);
}

bool OgreManager::addResourceForManager(const std::string &name, const std::string &grop, Ogre::ScriptLoader * loader)
{
	bool ret = false;
	const Ogre::StringVector& patterns = loader->getScriptPatterns();
	for (Ogre::StringVector::const_iterator p = patterns.begin(); p != patterns.end(); ++p)
	{
		Ogre::String temp(*p);
		size_t pos1 = temp.find('*');
		if (temp.npos != temp.find('*'))
		{
			temp = temp.substr(pos1 + 1);
		}

		if (name.npos != name.rfind(temp))
		{
			Ogre::DataStreamPtr dstream =
				Ogre::ResourceGroupManager::getSingleton().openResource(
				name, grop);
			loader->parseScript(dstream, grop);
			ret = true;
		}
	}
	return ret;
}
NS_CC_END