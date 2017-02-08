#ifndef OGRE_BRIDGE_SCENE_MANAGER_H
#define OGRE_BRIDGE_SCENE_MANAGER_H


#include "cocos2d.h"

#include <Ogre.h>
#include <OgreSceneManager.h>

namespace Ogre {

	class BridgeRenderQueue : public RenderQueue
	{
		
		cocos2d::Mat4 *mCocosMatrix4;

	public:

		int mIndex;
		std::vector<RenderablePass> mSolidRenderable;
		std::vector<RenderablePass> mTransparentRenderable;

		std::vector<cocos2d::MeshCommand *> mMeshCommand;
		std::vector<cocos2d::GLProgramState *> mProgramState;


		BridgeRenderQueue() :mCocosMatrix4(NULL)
		{
		
			for(int i = 0;i < 10; i ++)
			{
					auto glProgram =cocos2d::GLProgramCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
					auto programstate = cocos2d::GLProgramState::create(glProgram);
					programstate->retain();
					mProgramState.push_back(programstate);
					mMeshCommand.push_back(new cocos2d::MeshCommand());
			}
		};

		virtual ~BridgeRenderQueue(){};

		void setCocosMatrix4(cocos2d::Mat4 *mat)
		{
			mCocosMatrix4 = mat;
		}

		cocos2d::Mat4 * getCocosMatrix4()
		{
			return mCocosMatrix4;
		}

		virtual void addRenderable(Renderable* pRend, uint8 groupID, ushort priority)
		{
			pRend->setCocosMatrix4(mCocosMatrix4);
			//RenderQueue::addRenderable(pRend, groupID, priority);

			Technique* pTech;

			// tell material it's been used
			if (!pRend->getMaterial().isNull())
				pRend->getMaterial()->touch();

			// Check material & technique supplied (the former since the default implementation
			// of getTechnique is based on it for backwards compatibility
			if(pRend->getMaterial().isNull() || !pRend->getTechnique())
			{
				// Use default base white
				MaterialPtr baseWhite = MaterialManager::getSingleton().getByName("BaseWhite");
				pTech = baseWhite->getTechnique(0);
			}
			else
				pTech = pRend->getTechnique();

			if (mRenderableListener)
			{
				// Allow listener to override technique and to abort
				if (!mRenderableListener->renderableQueued(pRend, groupID, priority, 
					&pTech, this))
					return; // rejected

				// tell material it's been used (incase changed)
				pTech->getParent()->touch();
			}
		

			   // Transparent and depth/colour settings mean depth sorting is required?
			// Note: colour write disabled with depth check/write enabled means
			//       setup depth buffer for other passes use.
			if (pTech->isTransparentSortingForced() || 
				(pTech->isTransparent() && 
				(!pTech->isDepthWriteEnabled() ||
				 !pTech->isDepthCheckEnabled() ||
				 pTech->hasColourWriteDisabled())))
			{
				Technique::PassIterator pi = pTech->getPassIterator();

				while (pi.hasMoreElements())
				{
					
					// Insert into transparent list
					mTransparentRenderable.push_back(RenderablePass(pRend, pi.getNext()));
				}
			}
			else
			{
				Technique::PassIterator pi = pTech->getPassIterator();

			/*	while (pi.hasMoreElements())
				{
				
					auto glProgram =cocos2d::GLProgramCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
					auto programstate = cocos2d::GLProgramState::create(glProgram);
					programstate->retain();
					mProgramState.push_back(programstate);
					mMeshCommand.push_back(new cocos2d::MeshCommand());
			
					mSolidRenderable.push_back(RenderablePass(pRend, pi.getNext()));
				}*/
			}
		}
	};

	/// Default scene manager
	class BridgeSceneManager : public SceneManager
	{
		cocos2d::GLProgram* mShaderProgram;
		BridgeRenderQueue *mBridgeRenderQueue;

	public:

		BridgeSceneManager(const String& name)
			: SceneManager(name)
			, mShaderProgram(NULL)
			,mBridgeRenderQueue(NULL)
		{}

		~BridgeSceneManager(){}

		void clearSceneManager();

		void initRenderQueue(void);

		void initSceneManager(Viewport *vPort, Camera *camera, cocos2d::GLProgram*);

		void _beforeRender();

		void _draw(MovableObject *object,cocos2d::Renderer *render);

		void _render();

		void _render(Ogre::RenderablePass &obj,cocos2d::Renderer *render,cocos2d::GLProgramState *state,cocos2d::MeshCommand *cmd);

		void _afterRender();

		const String& getTypeName(void) const;
	};

	class BridgeSceneManagerFactory : public SceneManagerFactory
	{
	protected:
		void initMetaData(void) const;

	public:
		BridgeSceneManagerFactory(){}

		~BridgeSceneManagerFactory() {}

		/// Factory type name
		static const String FACTORY_TYPE_NAME;
		SceneManager* createInstance(const String& instanceName)
		{
			return OGRE_NEW BridgeSceneManager(instanceName);
		}
		void destroyInstance(SceneManager* instance)
		{
			OGRE_DELETE instance;
		}
	};
}
#endif//OGREAPP_H
