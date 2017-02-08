#include "OgreBridgeSceneManager.h"

#include "cocos2d.h"


#include <Ogre.h>
#include <OgreCodec.h>
#include "components/OgreBridgeBufferManager.h"

USING_NS_CC;

namespace Ogre {


	//-----------------------------------------------------------------------
	const String BridgeSceneManagerFactory::FACTORY_TYPE_NAME = "BridgeSceneManager";
	//-----------------------------------------------------------------------


	void BridgeSceneManager::clearSceneManager()
	{
		mCurrentViewport = NULL;
		mCameraInProgress = NULL;

		Root::getSingleton()._popCurrentSceneManager(this);
		mActiveQueuedRenderableVisitor->targetSceneMgr = NULL;
		mAutoParamDataSource->setCurrentSceneManager(NULL);
	}

	//-----------------------------------------------------------------------
	void BridgeSceneManager::initRenderQueue(void)
	{
		mBridgeRenderQueue = OGRE_NEW BridgeRenderQueue();
		mRenderQueue = mBridgeRenderQueue;
		// init render queues that do not need shadows
		mRenderQueue->getQueueGroup(RENDER_QUEUE_BACKGROUND)->setShadowsEnabled(false);
		mRenderQueue->getQueueGroup(RENDER_QUEUE_OVERLAY)->setShadowsEnabled(false);
		mRenderQueue->getQueueGroup(RENDER_QUEUE_SKIES_EARLY)->setShadowsEnabled(false);
		mRenderQueue->getQueueGroup(RENDER_QUEUE_SKIES_LATE)->setShadowsEnabled(false);
	}

	void BridgeSceneManager::initSceneManager(Viewport *vPort, Camera *camera, cocos2d::GLProgram *program)
	{
		mShaderProgram = program;
		mCurrentViewport = vPort;
		mCameraInProgress = camera;

		initRenderQueue();
		Root::getSingleton()._pushCurrentSceneManager(this);
		mActiveQueuedRenderableVisitor->targetSceneMgr = this;
		mAutoParamDataSource->setCurrentSceneManager(this);
	}

	void BridgeSceneManager::_beforeRender()
	{
		
	}

	void BridgeSceneManager::_draw(MovableObject *object,cocos2d::Renderer *render)
	{
		mBridgeRenderQueue->mSolidRenderable.clear();
		mBridgeRenderQueue->mTransparentRenderable.clear();

		//mBridgeRenderQueue->setCocosMatrix4(mat);
		//object->_notifyCurrentCamera(mCameraInProgress);
		object->_updateRenderQueue(mRenderQueue);
		//mBridgeRenderQueue->setCocosMatrix4(NULL);

		mBridgeRenderQueue->mIndex = 0;
		int size = mBridgeRenderQueue->mTransparentRenderable.size();
		for(int i = 0;i < size ; i++)
		{
			auto program = mBridgeRenderQueue->mProgramState[i];
			auto meshCommand = mBridgeRenderQueue->mMeshCommand[i];
			_render(mBridgeRenderQueue->mTransparentRenderable[i],render,program,meshCommand);
		}
	}

	void BridgeSceneManager::_render()
	{
		if (mShaderProgram && mRenderQueue)
		{
            mShaderProgram->use();
	

			int size = mBridgeRenderQueue->mTransparentRenderable.size();
			for(int i = 0;i < size ; i++)
			{
				auto program = mBridgeRenderQueue->mProgramState[i];
				auto meshCommand = mBridgeRenderQueue->mMeshCommand[i];
				//_render(mBridgeRenderQueue->mTransparentRenderable[i],NULL,program,meshCommand);
			}
		}
	}

	void BridgeSceneManager::_render(Ogre::RenderablePass &obj,cocos2d::Renderer *render,cocos2d::GLProgramState *programstate,cocos2d::MeshCommand *cmd)
	{
		if (mShaderProgram && mRenderQueue)
		{
            mShaderProgram->use();
			const Pass *pass = obj.pass;
			Renderable *rend = obj.renderable;
			
			cocos2d::BlendFunc _blend;
			_blend.src = pass->getSourceBlendFactor();
			_blend.dst = pass->getDestBlendFactor();
			GL::blendFunc(_blend.src, _blend.dst);
			// Give SM a chance to eliminate
			Pass::ConstTextureUnitStateIterator texIter =  pass->getTextureUnitStateIterator();
			size_t unit = 0;
			// Reset the shadow texture index for each pass

			/*while(texIter.hasMoreElements())
			{
				TextureUnitState* pTex = texIter.getNext();
		
				if (pTex->getContentType() == TextureUnitState::CONTENT_COMPOSITOR)
				{
					CompositorChain* currentChain = _getActiveCompositorChain();
					if (!currentChain)
					{
						OGRE_EXCEPT(Exception::ERR_INVALID_STATE,
							"A pass that wishes to reference a compositor texture "
							"attempted to render in a pipeline without a compositor",
							"SceneManager::_setPass");
					}
					CompositorInstance* refComp = currentChain->getCompositor(pTex->getReferencedCompositorName());
					if (refComp == 0)
					{
						OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
							"Invalid compositor content_type compositor name",
							"SceneManager::_setPass");
					}
					Ogre::TexturePtr refTex = refComp->getTextureInstance(
						pTex->getReferencedTextureName(), pTex->getReferencedMRTIndex());
					if (refTex.isNull())
					{
						OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
							"Invalid compositor content_type texture name",
							"SceneManager::_setPass");
					}
					pTex->_setTexturePtr(refTex);
				}
				//mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
				++unit;
			}
			// Disable remaining texture units
			mDestRenderSystem->_disableTextureUnitsFrom(pass->getNumTextureUnitStates());
			*/
		
			// Culling mode
			/*if (isShadowTechniqueTextureBased() 
				&& mIlluminationStage == IRS_RENDER_TO_TEXTURE
				&& mShadowCasterRenderBackFaces
				&& pass->getCullingMode() == CULL_CLOCKWISE)
			{
				// render back faces into shadow caster, can help with depth comparison
				mPassCullingMode = CULL_ANTICLOCKWISE;
			}
			else
			{
				mPassCullingMode = pass->getCullingMode();
			}
			//mDestRenderSystem->_setCullingMode(mPassCullingMode);
		
	
			GLenum cullMode;

			switch( mPassCullingMode )
			{
				case CULL_NONE:
					glDisable(GL_CULL_FACE);
					CHECK_GL_ERROR_DEBUG();;
				default:
				case CULL_CLOCKWISE:
						cullMode = GL_BACK;
					break;
				case CULL_ANTICLOCKWISE:
						cullMode = GL_BACK;
					break;
			}

			glEnable(GL_CULL_FACE);
			CHECK_GL_ERROR_DEBUG();;
			glCullFace(cullMode);
			CHECK_GL_ERROR_DEBUG();;
	
	*/
			unsigned short numMatrices;
			RenderOperation ro;

			//OgreProfileBeginGPUEvent("Material: " + pass->getParent()->getParent()->getName());
			// Set up rendering operation
			// I know, I know, const_cast is nasty but otherwise it requires all internal
			// state of the Renderable assigned to the rop to be mutable
			const_cast<Renderable*>(rend)->getRenderOperation(ro);
			ro.srcRenderable = rend;

			GpuProgram* vprog = pass->hasVertexProgram() ? pass->getVertexProgram().get() : 0;

			bool passTransformState = true;

			if (vprog)
			{
				passTransformState = vprog->getPassTransformStates();
			}

			// Set world transformation
			numMatrices = rend->getNumWorldTransforms();

			if (numMatrices > 0)
			{
				rend->getWorldTransforms(mTempXform);

				if (mCameraRelativeRendering && !rend->getUseIdentityView())
				{
					for (unsigned short i = 0; i < numMatrices; ++i)
					{
						mTempXform[i].setTrans(mTempXform[i].getTrans() - mCameraRelativePosition);
					}
				}

				/*if (passTransformState)
				{
					if (numMatrices > 1)
					{
						mDestRenderSystem->_setWorldMatrices(mTempXform, numMatrices);
					}
					else 
					{
						mDestRenderSystem->_setWorldMatrix(*mTempXform);
					}
				}*/
			}

			// Issue view / projection changes if any
			//useRenderableViewProjMode(rend, passTransformState);

			// mark per-object params as dirty
			//mGpuParamsDirty |= (uint16)GPV_PER_OBJECT;

			int cctextureId;
			int vecGlBufferId;

			if (!mSuppressRenderStateChanges)
			{
				bool passSurfaceAndLightParams = true;
							
						
				
				// Reissue any texture gen settings which are dependent on view matrix
				Pass::ConstTextureUnitStateIterator texIter = pass->getTextureUnitStateIterator();
				size_t unit = 0;
				while (texIter.hasMoreElements())
				{
					TextureUnitState* pTex = texIter.getNext();
						
					//mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
					//if (pTex->hasViewRelativeTextureCoordinateGeneration())
					//{
					//	mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
					//}
					//++unit;

						
					//GLES2BridgeTexture tex = pTex->_getTexturePtr();
					//cctextureId = tex->getGLID();
					//glBindTexture(GL_TEXTURE_2D, cctextureId);
				}

				/*// Sort out normalisation
				// Assume first world matrix representative - shaders that use multiple
				// matrices should control renormalisation themselves
				if ((pass->getNormaliseNormals() || mNormaliseNormalsOnScale)
					&& mTempXform[0].hasScale())
					mDestRenderSystem->setNormaliseNormals(true);
				else
					mDestRenderSystem->setNormaliseNormals(false);

				// Sort out negative scaling
				// Assume first world matrix representative 
				if (mFlipCullingOnNegativeScale)
				{
					CullingMode cullMode = mPassCullingMode;

					if (mTempXform[0].hasNegativeScale())
					{
						switch (mPassCullingMode)
						{
						case CULL_CLOCKWISE:
							cullMode = CULL_ANTICLOCKWISE;
							break;
						case CULL_ANTICLOCKWISE:
							cullMode = CULL_CLOCKWISE;
							break;
						case CULL_NONE:
							break;
						};
					}

					// this also copes with returning from negative scale in previous render op
					// for same pass
					if (cullMode != mDestRenderSystem->_getCullingMode())
						mDestRenderSystem->_setCullingMode(cullMode);
				}

				// Set up the solid / wireframe override
				// Precedence is Camera, Object, Material
				// Camera might not override object if not overrideable
				PolygonMode reqMode = pass->getPolygonMode();
				if (pass->getPolygonModeOverrideable() && rend->getPolygonModeOverrideable())
				{
					PolygonMode camPolyMode = mCameraInProgress->getPolygonMode();
					// check camera detial only when render detail is overridable
					if (reqMode > camPolyMode)
					{
						// only downgrade detail; if cam says wireframe we don't go up to solid
						reqMode = camPolyMode;
					}
				}
				mDestRenderSystem->_setPolygonMode(reqMode);*/
				// optional light scissoring
		
		


				void* pBufferData = 0;
				const VertexDeclaration::VertexElementList& decl =
					ro.vertexData->vertexDeclaration->getElements();
				VertexDeclaration::VertexElementList::const_iterator elem, elemEnd;
				elemEnd = decl.end();

				for (elem = decl.begin(); elem != elemEnd; ++elem)
				{
					if (!ro.vertexData->vertexBufferBinding->isBufferBound(elem->getSource()))
						continue; // skip unbound elements
					CHECK_GL_ERROR_DEBUG();;

					HardwareVertexBufferSharedPtr vertexBuffer =
						ro.vertexData->vertexBufferBinding->getBuffer(elem->getSource());

					GLuint bufferId = static_cast<const BridgeHardwareVertexBuffer*>(vertexBuffer.get())->getGLBufferId();

					vecGlBufferId = bufferId;
					//glBindBuffer(GL_ARRAY_BUFFER, bufferId);
					//_bindGLBuffer(GL_ARRAY_BUFFER,bufferId);

					pBufferData = ((char *)NULL + (elem->getOffset())) ;

					if (ro.vertexData->vertexStart)
					{
						pBufferData = static_cast<char*>(pBufferData)+ro.vertexData->vertexStart * vertexBuffer->getVertexSize();
					}

						
					GLenum type = 0;
					GLsizei size = 0;
					GLboolean normalised = GL_FALSE;
						
					const char* attribute_name = NULL;
					switch(elem->getType())
					{
						case VET_FLOAT1:
							type =  GL_FLOAT;
							size = 1;
							normalised = GL_FALSE;
							break;
						case VET_FLOAT2:
							type =  GL_FLOAT;
							size = 2;
							normalised = GL_FALSE;
							break;
						case VET_FLOAT3:
							type =  GL_FLOAT;
							size = 3;
							normalised = GL_FALSE;
							break;
						case VET_FLOAT4:
							type =  GL_FLOAT;
							size = 4;
							normalised = GL_FALSE;
							break;
						case VET_SHORT1:
							type =  GL_SHORT;
							size = 1;
							normalised = GL_FALSE;
							break;
						case VET_SHORT2:
							type =  GL_SHORT;
							size = 2;
							normalised = GL_FALSE;
							break;
						case VET_SHORT3:
							type =  GL_SHORT;
							size = 3;
							normalised = GL_FALSE;
							break;
						case VET_SHORT4:
							type =  GL_SHORT;
							size = 4;
							normalised = GL_FALSE;
							break;
						case VET_COLOUR:
						case VET_COLOUR_ABGR:
						case VET_COLOUR_ARGB:
						case VET_UBYTE4:
							size = 4;
							normalised = GL_TRUE;
							type =  GL_UNSIGNED_BYTE;
							break;
						default:
							break;
					};

						
					switch(elem->getSemantic())
					{
					case VES_POSITION:
						attribute_name = GLProgram::ATTRIBUTE_NAME_POSITION;
						break;
					case VES_BLEND_WEIGHTS:
						attribute_name = GLProgram::ATTRIBUTE_NAME_BLEND_WEIGHT;
						break;
					case VES_BLEND_INDICES:
						attribute_name = GLProgram::ATTRIBUTE_NAME_BLEND_INDEX;
						break;
					case VES_NORMAL:
						attribute_name = GLProgram::ATTRIBUTE_NAME_NORMAL;
						break;
					case VES_DIFFUSE:
						attribute_name = GLProgram::ATTRIBUTE_NAME_COLOR;
						break;
					//case VES_SPECULAR:
					//	break;
					case VES_TEXTURE_COORDINATES:
						attribute_name = GLProgram::ATTRIBUTE_NAME_TEX_COORD;
						break;
					//case VES_BINORMAL:
					//	break;
					//case VES_TANGENT:
					//	break;
					}

		
					//int type = GLES2HardwareBufferManager::getGLType(elem->getType());
					GLsizei stride = static_cast<GLsizei>(vertexBuffer->getVertexSize());

					programstate->setVertexAttribPointer(attribute_name,
						size,
						type,
						normalised,
						stride,
						(GLvoid*)pBufferData
					);
				}

				if (ro.useIndexes)
				{
					GLuint bufferId;
					//===GLuint bufferId = static_cast<GLES2HardwareIndexBuffer*>(ro.indexData->indexBuffer.get())->getGLBufferId();
					//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);

					pBufferData =((char *)NULL + (ro.indexData->indexStart *
						ro.indexData->indexBuffer->getIndexSize())) ;

		
					cmd->init(1000, cctextureId, programstate, _blend,vecGlBufferId,bufferId,ro.operationType,ro.indexData->indexBuffer->getType(),ro.indexData->indexCount,*((cocos2d::Mat4*)mTempXform));
				 
					//support tint and fade
					cmd->setDisplayColor(Vec4(1.0, 1.0, 1.0, 1.0));
					
				    cmd->genMaterialID(cctextureId, programstate, vecGlBufferId, bufferId, _blend);
    			    cmd->setTransparent(true);	
					//cmd->setDepthTestEnabled(false);
					//cmd->setDepthWriteEnabled(false);
				
					//cmd->preBatchDraw();
					//cmd->batchDraw();
					//cmd->postBatchDraw();
					//cmd->execute();
					render->addCommand(cmd);


					/*	mDestRenderSystem->_setDepthBufferFunction(pass->getDepthFunction());
		mDestRenderSystem->_setDepthBufferCheckEnabled(pass->getDepthCheckEnabled());
		mDestRenderSystem->_setDepthBufferWriteEnabled(pass->getDepthWriteEnabled());
		mDestRenderSystem->_setDepthBias(pass->getDepthBiasConstant(), 
			pass->getDepthBiasSlopeScale());*/

				//_meshCommand.setTransparent(mesh->_isTransparent);
					/*

					programstate->applyGLProgram(*((cocos2d::Mat4*)mTempXform));
					programstate->applyUniforms();





					glBindBuffer(GL_ARRAY_BUFFER,vecGlBufferId);
					programstate->applyAttributes();
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  bufferId);
					glDrawElements(ro.operationType, ro.indexData->indexCount, indexType,pBufferData);
					CHECK_GL_ERROR_DEBUG();
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

					CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
					CHECK_GL_ERROR_DEBUG();
					// Update derived depth bias
					*/

				}
				
			}
			
		}
	}

	void BridgeSceneManager::_afterRender()
	{
		
	}

	const String& BridgeSceneManager::getTypeName(void) const
	{
		return BridgeSceneManagerFactory::FACTORY_TYPE_NAME;
	}
	
	void BridgeSceneManagerFactory::initMetaData(void) const
	{
		mMetaData.typeName = FACTORY_TYPE_NAME;
		mMetaData.description = "The bridge scene manager";
		mMetaData.sceneTypeMask = ST_GENERIC;
		mMetaData.worldGeometrySupported = false;
	}
};


/*


	//-----------------------------------------------------------------------
	void BridgeSceneManager::renderSingleObject1(Renderable* rend, const Pass* pass,
		bool lightScissoringClipping, bool doLightIteration,
		const LightList* manualLightList)
	{
		return ;
		unsigned short numMatrices;
		RenderOperation ro;

		OgreProfileBeginGPUEvent("Material: " + pass->getParent()->getParent()->getName());
		// Set up rendering operation
		// I know, I know, const_cast is nasty but otherwise it requires all internal
		// state of the Renderable assigned to the rop to be mutable
		const_cast<Renderable*>(rend)->getRenderOperation(ro);
		ro.srcRenderable = rend;

		GpuProgram* vprog = pass->hasVertexProgram() ? pass->getVertexProgram().get() : 0;

		bool passTransformState = true;

		if (vprog)
		{
			passTransformState = vprog->getPassTransformStates();
		}

		// Set world transformation
		numMatrices = rend->getNumWorldTransforms();

		if (numMatrices > 0)
		{
			rend->getWorldTransforms(mTempXform);

			if (mCameraRelativeRendering && !rend->getUseIdentityView())
			{
				for (unsigned short i = 0; i < numMatrices; ++i)
				{
					mTempXform[i].setTrans(mTempXform[i].getTrans() - mCameraRelativePosition);
				}
			}

			if (passTransformState)
			{
				if (numMatrices > 1)
				{
					mDestRenderSystem->_setWorldMatrices(mTempXform, numMatrices);
				}
				else
				{
					mDestRenderSystem->_setWorldMatrix(*mTempXform);
				}

				if (mShaderProgram)
				{

					mShaderProgram->use();
					mShaderProgram->setUniformsForBuiltins(*((cocos2d::Mat4*)mTempXform));// rend->getCocosMatrix4()));
				}
			}
		}

		// Issue view / projection changes if any
		useRenderableViewProjMode(rend, passTransformState);

		// mark per-object params as dirty
		mGpuParamsDirty |= (uint16)GPV_PER_OBJECT;

		if (!mSuppressRenderStateChanges)
		{
			bool passSurfaceAndLightParams = true;

			if (pass->isProgrammable())
			{
				// Tell auto params object about the renderable change
				mAutoParamDataSource->setCurrentRenderable(rend);
				// Tell auto params object about the world matrices, eliminated query from renderable again
				mAutoParamDataSource->setWorldMatrices(mTempXform, numMatrices);
				if (vprog)
				{
					passSurfaceAndLightParams = vprog->getPassSurfaceAndLightStates();
				}
			}

			// Reissue any texture gen settings which are dependent on view matrix
			Pass::ConstTextureUnitStateIterator texIter = pass->getTextureUnitStateIterator();
			size_t unit = 0;
			while (texIter.hasMoreElements())
			{
				TextureUnitState* pTex = texIter.getNext();
				if (pTex->hasViewRelativeTextureCoordinateGeneration())
				{
					mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
				}
				++unit;
			}

			// Sort out normalisation
			// Assume first world matrix representative - shaders that use multiple
			// matrices should control renormalisation themselves
			if ((pass->getNormaliseNormals() || mNormaliseNormalsOnScale)
				&& mTempXform[0].hasScale())
				mDestRenderSystem->setNormaliseNormals(true);
			else
				mDestRenderSystem->setNormaliseNormals(false);

			// Sort out negative scaling
			// Assume first world matrix representative 
			if (mFlipCullingOnNegativeScale)
			{
				CullingMode cullMode = mPassCullingMode;

				if (mTempXform[0].hasNegativeScale())
				{
					switch (mPassCullingMode)
					{
					case CULL_CLOCKWISE:
						cullMode = CULL_ANTICLOCKWISE;
						break;
					case CULL_ANTICLOCKWISE:
						cullMode = CULL_CLOCKWISE;
						break;
					case CULL_NONE:
						break;
					};
				}

				// this also copes with returning from negative scale in previous render op
				// for same pass
				if (cullMode != mDestRenderSystem->_getCullingMode())
					mDestRenderSystem->_setCullingMode(cullMode);
			}

			// Set up the solid / wireframe override
			// Precedence is Camera, Object, Material
			// Camera might not override object if not overrideable
			PolygonMode reqMode = pass->getPolygonMode();
			if (pass->getPolygonModeOverrideable() && rend->getPolygonModeOverrideable())
			{
				PolygonMode camPolyMode = mCameraInProgress->getPolygonMode();
				// check camera detial only when render detail is overridable
				if (reqMode > camPolyMode)
				{
					// only downgrade detail; if cam says wireframe we don't go up to solid
					reqMode = camPolyMode;
				}
			}
			mDestRenderSystem->_setPolygonMode(reqMode);



			// no automatic light processing
			{
				// Even if manually driving lights, check light type passes
				bool skipBecauseOfLightType = false;
				if (pass->getRunOnlyForOneLightType())
				{
					if (!manualLightList ||
						(manualLightList->size() == 1 &&
						manualLightList->at(0)->getType() != pass->getOnlyLightType()))
					{
						skipBecauseOfLightType = true;
					}
				}

				if (!skipBecauseOfLightType)
				{
					fireRenderSingleObject(rend, pass, mAutoParamDataSource, manualLightList, mSuppressRenderStateChanges);
					// Do we need to update GPU program parameters?
					if (pass->isProgrammable())
					{
						// Do we have a manual light list?
						if (manualLightList)
						{
							useLightsGpuProgram(pass, manualLightList);
						}

					}

					// Use manual lights if present, and not using vertex programs that don't use fixed pipeline
					if (manualLightList &&
						pass->getLightingEnabled() && passSurfaceAndLightParams)
					{
						useLights(*manualLightList, pass->getMaxSimultaneousLights());
					}

					// optional light scissoring
					ClipResult scissored = CLIPPED_NONE;
					ClipResult clipped = CLIPPED_NONE;
					if (lightScissoringClipping && manualLightList && pass->getLightScissoringEnabled())
					{
						scissored = buildAndSetScissor(*manualLightList, mCameraInProgress);
					}
					if (lightScissoringClipping && manualLightList && pass->getLightClipPlanesEnabled())
					{
						clipped = buildAndSetLightClip(*manualLightList);
					}

					// don't bother rendering if clipped / scissored entirely
					if (scissored != CLIPPED_ALL && clipped != CLIPPED_ALL)
					{
						// issue the render op		
						// nfz: set up multipass rendering
						mDestRenderSystem->setCurrentPassIterationCount(pass->getPassIterationCount());
						// Finalise GPU parameter bindings
						updateGpuProgramParameters(pass);

						if (rend->preRender(this, mDestRenderSystem))
                        {
                            mDestRenderSystem->_render(ro);
                        }
						rend->postRender(this, mDestRenderSystem);
					}
					if (scissored == CLIPPED_SOME)
						resetScissor();
					if (clipped == CLIPPED_SOME)
						resetLightClip();

				} // !skipBecauseOfLightType
			}

		}
		else // mSuppressRenderStateChanges
		{
			fireRenderSingleObject(rend, pass, mAutoParamDataSource, NULL, mSuppressRenderStateChanges);
			// Just render
			mDestRenderSystem->setCurrentPassIterationCount(1);
			if (rend->preRender(this, mDestRenderSystem))
				mDestRenderSystem->_render(ro);
			rend->postRender(this, mDestRenderSystem);
		}

		// Reset view / projection changes if any
		resetViewProjMode(passTransformState);
		OgreProfileEndGPUEvent("Material: " + pass->getParent()->getParent()->getName());
	}


	//-----------------------------------------------------------------------
	void BridgeSceneManager::renderSingleObject(Renderable* rend, const Pass* pass,
		bool lightScissoringClipping, bool doLightIteration,
		const LightList* manualLightList)
	{
		unsigned short numMatrices;
		RenderOperation ro;

		OgreProfileBeginGPUEvent("Material: " + pass->getParent()->getParent()->getName());
		// Set up rendering operation
		// I know, I know, const_cast is nasty but otherwise it requires all internal
		// state of the Renderable assigned to the rop to be mutable
		const_cast<Renderable*>(rend)->getRenderOperation(ro);
		ro.srcRenderable = rend;

		GpuProgram* vprog = pass->hasVertexProgram() ? pass->getVertexProgram().get() : 0;

		bool passTransformState = true;

		if (vprog)
		{
			passTransformState = vprog->getPassTransformStates();
		}

		// Set world transformation
		numMatrices = rend->getNumWorldTransforms();

		if (numMatrices > 0)
		{
			rend->getWorldTransforms(mTempXform);

			if (mCameraRelativeRendering && !rend->getUseIdentityView())
			{
				for (unsigned short i = 0; i < numMatrices; ++i)
				{
					mTempXform[i].setTrans(mTempXform[i].getTrans() - mCameraRelativePosition);
				}
			}

			if (passTransformState)
			{
				if (numMatrices > 1)
				{
					mDestRenderSystem->_setWorldMatrices(mTempXform, numMatrices);
				}
				else
				{
					mDestRenderSystem->_setWorldMatrix(*mTempXform);
				}

				if (mShaderProgram)
				{

					mShaderProgram->use();
					mShaderProgram->setUniformsForBuiltins(*((cocos2d::Mat4*)mTempXform));// rend->getCocosMatrix4()));
				}
			}
		}

		// Issue view / projection changes if any
		useRenderableViewProjMode(rend, passTransformState);

		// mark per-object params as dirty
		mGpuParamsDirty |= (uint16)GPV_PER_OBJECT;

		if (!mSuppressRenderStateChanges)
		{
			bool passSurfaceAndLightParams = true;

			if (pass->isProgrammable())
			{
				// Tell auto params object about the renderable change
				mAutoParamDataSource->setCurrentRenderable(rend);
				// Tell auto params object about the world matrices, eliminated query from renderable again
				mAutoParamDataSource->setWorldMatrices(mTempXform, numMatrices);
				if (vprog)
				{
					passSurfaceAndLightParams = vprog->getPassSurfaceAndLightStates();
				}
			}

			// Reissue any texture gen settings which are dependent on view matrix
			Pass::ConstTextureUnitStateIterator texIter = pass->getTextureUnitStateIterator();
			size_t unit = 0;
			while (texIter.hasMoreElements())
			{
				TextureUnitState* pTex = texIter.getNext();
				if (pTex->hasViewRelativeTextureCoordinateGeneration())
				{
					mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
				}
				++unit;
			}

			// Sort out normalisation
			// Assume first world matrix representative - shaders that use multiple
			// matrices should control renormalisation themselves
			if ((pass->getNormaliseNormals() || mNormaliseNormalsOnScale)
				&& mTempXform[0].hasScale())
				mDestRenderSystem->setNormaliseNormals(true);
			else
				mDestRenderSystem->setNormaliseNormals(false);

			// Sort out negative scaling
			// Assume first world matrix representative 
			if (mFlipCullingOnNegativeScale)
			{
				CullingMode cullMode = mPassCullingMode;

				if (mTempXform[0].hasNegativeScale())
				{
					switch (mPassCullingMode)
					{
					case CULL_CLOCKWISE:
						cullMode = CULL_ANTICLOCKWISE;
						break;
					case CULL_ANTICLOCKWISE:
						cullMode = CULL_CLOCKWISE;
						break;
					case CULL_NONE:
						break;
					};
				}

				// this also copes with returning from negative scale in previous render op
				// for same pass
				if (cullMode != mDestRenderSystem->_getCullingMode())
					mDestRenderSystem->_setCullingMode(cullMode);
			}

			// Set up the solid / wireframe override
			// Precedence is Camera, Object, Material
			// Camera might not override object if not overrideable
			PolygonMode reqMode = pass->getPolygonMode();
			if (pass->getPolygonModeOverrideable() && rend->getPolygonModeOverrideable())
			{
				PolygonMode camPolyMode = mCameraInProgress->getPolygonMode();
				// check camera detial only when render detail is overridable
				if (reqMode > camPolyMode)
				{
					// only downgrade detail; if cam says wireframe we don't go up to solid
					reqMode = camPolyMode;
				}
			}
			mDestRenderSystem->_setPolygonMode(reqMode);

	

			// no automatic light processing
			{
				// Even if manually driving lights, check light type passes
				bool skipBecauseOfLightType = false;
				if (pass->getRunOnlyForOneLightType())
				{
					if (!manualLightList ||
						(manualLightList->size() == 1 &&
						manualLightList->at(0)->getType() != pass->getOnlyLightType()))
					{
						skipBecauseOfLightType = true;
					}
				}

				if (!skipBecauseOfLightType)
				{
					fireRenderSingleObject(rend, pass, mAutoParamDataSource, manualLightList, mSuppressRenderStateChanges);
					// Do we need to update GPU program parameters?
					if (pass->isProgrammable())
					{
						// Do we have a manual light list?
						if (manualLightList)
						{
							useLightsGpuProgram(pass, manualLightList);
						}

					}

					// Use manual lights if present, and not using vertex programs that don't use fixed pipeline
					if (manualLightList &&
						pass->getLightingEnabled() && passSurfaceAndLightParams)
					{
						useLights(*manualLightList, pass->getMaxSimultaneousLights());
					}

					// optional light scissoring
					ClipResult scissored = CLIPPED_NONE;
					ClipResult clipped = CLIPPED_NONE;
					if (lightScissoringClipping && manualLightList && pass->getLightScissoringEnabled())
					{
						scissored = buildAndSetScissor(*manualLightList, mCameraInProgress);
					}
					if (lightScissoringClipping && manualLightList && pass->getLightClipPlanesEnabled())
					{
						clipped = buildAndSetLightClip(*manualLightList);
					}

					// don't bother rendering if clipped / scissored entirely
					if (scissored != CLIPPED_ALL && clipped != CLIPPED_ALL)
					{
						// issue the render op		
						// nfz: set up multipass rendering
						mDestRenderSystem->setCurrentPassIterationCount(pass->getPassIterationCount());
						// Finalise GPU parameter bindings
						updateGpuProgramParameters(pass);

						if (rend->preRender(this, mDestRenderSystem))
                        {
                           //mDestRenderSystem->_render(ro);

							void* pBufferData = 0;
							const VertexDeclaration::VertexElementList& decl =
								ro.vertexData->vertexDeclaration->getElements();
							VertexDeclaration::VertexElementList::const_iterator elem, elemEnd;
							elemEnd = decl.end();

							for (elem = decl.begin(); elem != elemEnd; ++elem)
							{
								if (!ro.vertexData->vertexBufferBinding->isBufferBound(elem->getSource()))
									continue; // skip unbound elements
								CHECK_GL_ERROR_DEBUG();;

								HardwareVertexBufferSharedPtr vertexBuffer =
									ro.vertexData->vertexBufferBinding->getBuffer(elem->getSource());

								GLuint bufferId = static_cast<const BridgeHardwareVertexBuffer*>(vertexBuffer.get())->getGLBufferId();

								glBindBuffer(GL_ARRAY_BUFFER, bufferId);
								//_bindGLBuffer(GL_ARRAY_BUFFER,bufferId);

								pBufferData = ((char *)NULL + (elem->getOffset())) ;

								if (ro.vertexData->vertexStart)
								{
									pBufferData = static_cast<char*>(pBufferData)+ro.vertexData->vertexStart * vertexBuffer->getVertexSize();
								}

								VertexElementSemantic sem = elem->getSemantic();

								unsigned short typeCount = VertexElement::getTypeCount(elem->getType());
								GLboolean normalised = GL_FALSE;
								bool multitexturing = false;//(getCapabilities()->getNumTextureUnits() > 1);

								bool isCustomAttrib = false;
								GLuint attrib = 0;

								switch (elem->getType())
								{
								case VET_COLOUR:
								case VET_COLOUR_ABGR:
								case VET_COLOUR_ARGB:
									// Because GL takes these as a sequence of single unsigned bytes, count needs to be 4
									// VertexElement::getTypeCount treats them as 1 (RGBA)
									// Also need to normalise the fixed-point data
									typeCount = 4;
									normalised = GL_TRUE;
									break;
								default:
									break;
								};

								if (!isCustomAttrib)
								{
									// fixed-function & builtin attribute support
									switch (sem)
									{
									case VES_POSITION:
										attrib = 0;
										break;
										//case VES_NORMAL:
										//	break;
									case VES_DIFFUSE:
										attrib = 1;
										break;
										//case VES_SPECULAR:
										//	break;
									case VES_TEXTURE_COORDINATES:
										attrib = 2;
										break;
									default:
										attrib = 12;
										break;
									};
								}

								int type = GLES2HardwareBufferManager::getGLType(elem->getType());
								GLsizei size = static_cast<GLsizei>(vertexBuffer->getVertexSize());

								CHECK_GL_ERROR_DEBUG();
									glVertexAttribPointer(attrib,
									typeCount,
									type,
									normalised,
									size,
									pBufferData);
								CHECK_GL_ERROR_DEBUG();;
							}

							// Find the correct type to render
							GLint primType;
							switch (ro.operationType)
							{
							case RenderOperation::OT_POINT_LIST:
								primType = GL_POINTS;
								break;
							case RenderOperation::OT_LINE_LIST:
								primType = GL_LINES;
								break;
							case RenderOperation::OT_LINE_STRIP:
								primType = GL_LINE_STRIP;
								break;
							default:
							case RenderOperation::OT_TRIANGLE_LIST:
								primType = GL_TRIANGLES;
								break;
							case RenderOperation::OT_TRIANGLE_STRIP:
								primType = GL_TRIANGLE_STRIP;
								break;
							case RenderOperation::OT_TRIANGLE_FAN:
								primType = GL_TRIANGLE_FAN;
								break;
							}

							if (ro.useIndexes)
							{
								GLuint bufferId = static_cast<GLES2HardwareIndexBuffer*>(ro.indexData->indexBuffer.get())->getGLBufferId();
								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);

								pBufferData =((char *)NULL + (ro.indexData->indexStart *
									ro.indexData->indexBuffer->getIndexSize())) ;

								GLenum indexType = (ro.indexData->indexBuffer->getType() == HardwareIndexBuffer::IT_16BIT) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;

		
								// Update derived depth bias
				
								CHECK_GL_ERROR_DEBUG();;
								GLenum primitive =  primType ;
								glDrawElements(primitive, ro.indexData->indexCount, indexType, pBufferData);
								CHECK_GL_ERROR_DEBUG();;
							}
                        }
						rend->postRender(this, mDestRenderSystem);
					}
					if (scissored == CLIPPED_SOME)
						resetScissor();
					if (clipped == CLIPPED_SOME)
						resetLightClip();

				} // !skipBecauseOfLightType
			}

		}
		else // mSuppressRenderStateChanges
		{
			fireRenderSingleObject(rend, pass, mAutoParamDataSource, NULL, mSuppressRenderStateChanges);
			// Just render
			mDestRenderSystem->setCurrentPassIterationCount(1);
			if (rend->preRender(this, mDestRenderSystem))
			{
				mDestRenderSystem->_render(ro);
			}
			rend->postRender(this, mDestRenderSystem);
		}

		// Reset view / projection changes if any
		resetViewProjMode(passTransformState);
		OgreProfileEndGPUEvent("Material: " + pass->getParent()->getParent()->getName());
	}

	void BridgeSceneManager::_render11()
	{
		return ;
		if (mShaderProgram && mRenderQueue)
		{
            mShaderProgram->use();
	

			auto getBlend = [](int ogreBlend)
			{
				switch (ogreBlend)
				{
					case SBF_ONE:
						return GL_ONE;
					case SBF_ZERO:
						return GL_ZERO;
					case SBF_DEST_COLOUR:
						return GL_DST_COLOR;
					case SBF_SOURCE_COLOUR:
						return GL_SRC_COLOR;
					case SBF_ONE_MINUS_DEST_COLOUR:
						return GL_ONE_MINUS_DST_COLOR;
					case SBF_ONE_MINUS_SOURCE_COLOUR:
						return GL_ONE_MINUS_SRC_COLOR;
					case SBF_DEST_ALPHA:
						return GL_DST_ALPHA;
					case SBF_SOURCE_ALPHA:
						return GL_SRC_ALPHA;
					case SBF_ONE_MINUS_DEST_ALPHA:
						return GL_ONE_MINUS_DST_ALPHA;
					case SBF_ONE_MINUS_SOURCE_ALPHA:
						return GL_ONE_MINUS_SRC_ALPHA;
				};
				return GL_ONE;
			};

			
			for(auto obj : mBridgeRenderQueue->mTransparentRenderable)
			{
				// Give SM a chance to eliminate
			
				GLenum sourceBlend = getBlend(obj.pass->getSourceBlendFactor());
				GLenum destBlend = getBlend(obj.pass->getDestBlendFactor());


				const Pass *pass = obj.pass;
				Renderable *rend = obj.renderable;

			    CHECK_GL_ERROR_DEBUG();;
		
				if(pass->getSourceBlendFactor() == SBF_ONE && pass->getDestBlendFactor() == SBF_ZERO)
				{
					glDisable(GL_BLEND);
					CHECK_GL_ERROR_DEBUG();;
				}
				else
				{
					glEnable(GL_BLEND);
					CHECK_GL_ERROR_DEBUG();;
					glBlendFunc(sourceBlend, destBlend);
					CHECK_GL_ERROR_DEBUG();;
				}
        
				GLint func = GL_FUNC_ADD;
				switch(pass->getSceneBlendingOperation())
				{
				case SBO_ADD:
					func = GL_FUNC_ADD;
					break;
				case SBO_SUBTRACT:
					func = GL_FUNC_SUBTRACT;
					break;
				case SBO_REVERSE_SUBTRACT:
					func = GL_FUNC_REVERSE_SUBTRACT;
					break;
				case SBO_MIN:
		#if GL_EXT_blend_minmax
					func = GL_MIN_EXT;
		#endif
					break;
				case SBO_MAX:
		#if GL_EXT_blend_minmax
					func = GL_MAX_EXT;
		#endif
					break;
				}

				glBlendEquation(func);
				CHECK_GL_ERROR_DEBUG();;
			
			
				
				Pass::ConstTextureUnitStateIterator texIter =  pass->getTextureUnitStateIterator();
				size_t unit = 0;
				// Reset the shadow texture index for each pass

				while(texIter.hasMoreElements())
				{
					TextureUnitState* pTex = texIter.getNext();
		
					if (pTex->getContentType() == TextureUnitState::CONTENT_COMPOSITOR)
					{
						CompositorChain* currentChain = _getActiveCompositorChain();
						if (!currentChain)
						{
							OGRE_EXCEPT(Exception::ERR_INVALID_STATE,
								"A pass that wishes to reference a compositor texture "
								"attempted to render in a pipeline without a compositor",
								"SceneManager::_setPass");
						}
						CompositorInstance* refComp = currentChain->getCompositor(pTex->getReferencedCompositorName());
						if (refComp == 0)
						{
							OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
								"Invalid compositor content_type compositor name",
								"SceneManager::_setPass");
						}
						Ogre::TexturePtr refTex = refComp->getTextureInstance(
							pTex->getReferencedTextureName(), pTex->getReferencedMRTIndex());
						if (refTex.isNull())
						{
							OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
								"Invalid compositor content_type texture name",
								"SceneManager::_setPass");
						}
						pTex->_setTexturePtr(refTex);
					}
					//mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
					++unit;

			}
			// Disable remaining texture units
			mDestRenderSystem->_disableTextureUnitsFrom(pass->getNumTextureUnitStates());

		
			// Culling mode
			if (isShadowTechniqueTextureBased() 
				&& mIlluminationStage == IRS_RENDER_TO_TEXTURE
				&& mShadowCasterRenderBackFaces
				&& pass->getCullingMode() == CULL_CLOCKWISE)
			{
				// render back faces into shadow caster, can help with depth comparison
				mPassCullingMode = CULL_ANTICLOCKWISE;
			}
			else
			{
				mPassCullingMode = pass->getCullingMode();
			}
			//mDestRenderSystem->_setCullingMode(mPassCullingMode);
		
	
			GLenum cullMode;

			switch( mPassCullingMode )
			{
				case CULL_NONE:
					glDisable(GL_CULL_FACE);
					CHECK_GL_ERROR_DEBUG();;
				default:
				case CULL_CLOCKWISE:
						cullMode = GL_BACK;
					break;
				case CULL_ANTICLOCKWISE:
						cullMode = GL_BACK;
					break;
			}

			glEnable(GL_CULL_FACE);
			CHECK_GL_ERROR_DEBUG();;
			glCullFace(cullMode);
			CHECK_GL_ERROR_DEBUG();;

			GLenum mPolygonMode;

			switch(pass->getPolygonMode())
			{
			case PM_POINTS:
				mPolygonMode = GL_POINTS;
				break;
			case PM_WIREFRAME:
				mPolygonMode = GL_LINE_STRIP;
				break;
			default:
			case PM_SOLID:
				mPolygonMode = GL_FILL;
				break;
			}

			
			//==================renderSingleObject(obj.renderable,obj.pass,true,true,NULL);
				
			auto glProgram =cocos2d::GLProgramCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
			auto programstate = cocos2d::GLProgramState::create(glProgram);
		
			//this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

				unsigned short numMatrices;
				RenderOperation ro;

				OgreProfileBeginGPUEvent("Material: " + pass->getParent()->getParent()->getName());
				// Set up rendering operation
				// I know, I know, const_cast is nasty but otherwise it requires all internal
				// state of the Renderable assigned to the rop to be mutable
				const_cast<Renderable*>(rend)->getRenderOperation(ro);
				ro.srcRenderable = rend;

				GpuProgram* vprog = pass->hasVertexProgram() ? pass->getVertexProgram().get() : 0;

				bool passTransformState = true;

				if (vprog)
				{
					passTransformState = vprog->getPassTransformStates();
				}

				// Set world transformation
				numMatrices = rend->getNumWorldTransforms();

				if (numMatrices > 0)
				{
					rend->getWorldTransforms(mTempXform);

					if (mCameraRelativeRendering && !rend->getUseIdentityView())
					{
						for (unsigned short i = 0; i < numMatrices; ++i)
						{
							mTempXform[i].setTrans(mTempXform[i].getTrans() - mCameraRelativePosition);
						}
					}

					if (passTransformState)
					{
						if (numMatrices > 1)
						{
							mDestRenderSystem->_setWorldMatrices(mTempXform, numMatrices);
						}
						else
						{
							mDestRenderSystem->_setWorldMatrix(*mTempXform);
						}
					}

					
					if (mShaderProgram)
					{

						mShaderProgram->use();
						//mShaderProgram->setUniformsForBuiltins(*((cocos2d::Mat4*)mTempXform));// rend->getCocosMatrix4()));
					}
				}

				// Issue view / projection changes if any
				useRenderableViewProjMode(rend, passTransformState);

				// mark per-object params as dirty
				mGpuParamsDirty |= (uint16)GPV_PER_OBJECT;

				int cctextureId;
				if (!mSuppressRenderStateChanges)
				{
					bool passSurfaceAndLightParams = true;


					// Reissue any texture gen settings which are dependent on view matrix
					Pass::ConstTextureUnitStateIterator texIter = pass->getTextureUnitStateIterator();
					size_t unit = 0;
					while (texIter.hasMoreElements())
					{
						TextureUnitState* pTex = texIter.getNext();
						
						mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
						if (pTex->hasViewRelativeTextureCoordinateGeneration())
						{
						//	mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
						}
						++unit;

						
						GLES2BridgeTexture tex = pTex->_getTexturePtr();
						cctextureId = tex->getGLID();
						glBindTexture(GL_TEXTURE_2D, cctextureId);
					}

					// Sort out normalisation
					// Assume first world matrix representative - shaders that use multiple
					// matrices should control renormalisation themselves
					if ((pass->getNormaliseNormals() || mNormaliseNormalsOnScale)
						&& mTempXform[0].hasScale())
						mDestRenderSystem->setNormaliseNormals(true);
					else
						mDestRenderSystem->setNormaliseNormals(false);

					// Sort out negative scaling
					// Assume first world matrix representative 
					if (mFlipCullingOnNegativeScale)
					{
						CullingMode cullMode = mPassCullingMode;

						if (mTempXform[0].hasNegativeScale())
						{
							switch (mPassCullingMode)
							{
							case CULL_CLOCKWISE:
								cullMode = CULL_ANTICLOCKWISE;
								break;
							case CULL_ANTICLOCKWISE:
								cullMode = CULL_CLOCKWISE;
								break;
							case CULL_NONE:
								break;
							};
						}

						// this also copes with returning from negative scale in previous render op
						// for same pass
						if (cullMode != mDestRenderSystem->_getCullingMode())
							mDestRenderSystem->_setCullingMode(cullMode);
					}

					// Set up the solid / wireframe override
					// Precedence is Camera, Object, Material
					// Camera might not override object if not overrideable
					PolygonMode reqMode = pass->getPolygonMode();
					if (pass->getPolygonModeOverrideable() && rend->getPolygonModeOverrideable())
					{
						PolygonMode camPolyMode = mCameraInProgress->getPolygonMode();
						// check camera detial only when render detail is overridable
						if (reqMode > camPolyMode)
						{
							// only downgrade detail; if cam says wireframe we don't go up to solid
							reqMode = camPolyMode;
						}
					}
					mDestRenderSystem->_setPolygonMode(reqMode);
					// optional light scissoring
					ClipResult scissored = CLIPPED_NONE;
					ClipResult clipped = CLIPPED_NONE;
							
				
					if (rend->preRender(this, mDestRenderSystem))
					{
						//mDestRenderSystem->_render(ro);
						
						void* pBufferData = 0;
						const VertexDeclaration::VertexElementList& decl =
							ro.vertexData->vertexDeclaration->getElements();
						VertexDeclaration::VertexElementList::const_iterator elem, elemEnd;
						elemEnd = decl.end();

						for (elem = decl.begin(); elem != elemEnd; ++elem)
						{
							if (!ro.vertexData->vertexBufferBinding->isBufferBound(elem->getSource()))
								continue; // skip unbound elements
							CHECK_GL_ERROR_DEBUG();;

							HardwareVertexBufferSharedPtr vertexBuffer =
								ro.vertexData->vertexBufferBinding->getBuffer(elem->getSource());

							GLuint bufferId = static_cast<const BridgeHardwareVertexBuffer*>(vertexBuffer.get())->getGLBufferId();

							glBindBuffer(GL_ARRAY_BUFFER, bufferId);
							//_bindGLBuffer(GL_ARRAY_BUFFER,bufferId);

							pBufferData = ((char *)NULL + (elem->getOffset())) ;

							if (ro.vertexData->vertexStart)
							{
								pBufferData = static_cast<char*>(pBufferData)+ro.vertexData->vertexStart * vertexBuffer->getVertexSize();
							}

							VertexElementSemantic sem = elem->getSemantic();

							unsigned short typeCount = VertexElement::getTypeCount(elem->getType());
							GLboolean normalised = GL_FALSE;
							bool multitexturing = false;//(getCapabilities()->getNumTextureUnits() > 1);

							bool isCustomAttrib = false;
							GLuint attrib = 0;

							switch (elem->getType())
							{
							case VET_COLOUR:
							case VET_COLOUR_ABGR:
							case VET_COLOUR_ARGB:
								// Because GL takes these as a sequence of single unsigned bytes, count needs to be 4
								// VertexElement::getTypeCount treats them as 1 (RGBA)
								// Also need to normalise the fixed-point data
								typeCount = 4;
								normalised = GL_TRUE;
								break;
							default:
								break;
							};


							std::string key;

							if (!isCustomAttrib)
							{
								// fixed-function & builtin attribute support
								switch (sem)
								{
								case VES_POSITION:
									attrib = 0;
									key = GLProgram::ATTRIBUTE_NAME_POSITION;
									break;
									//case VES_NORMAL:
									//	break;
								case VES_DIFFUSE:
									attrib = 1;
									key = GLProgram::ATTRIBUTE_NAME_COLOR;
									break;
									//case VES_SPECULAR:
									//	break;
								case VES_TEXTURE_COORDINATES:
									attrib = 2;
									key = GLProgram::ATTRIBUTE_NAME_TEX_COORD;
									break;
								default:
									attrib = 12;
									break;
								};
							}

							int type = GLES2HardwareBufferManager::getGLType(elem->getType());
							GLsizei size = static_cast<GLsizei>(vertexBuffer->getVertexSize());

							programstate->setVertexAttribPointer(key,
								typeCount,
								type,
								normalised,
								size,
								(GLvoid*)pBufferData
							);
						}

					
						// Find the correct type to render
						GLint primType;
						switch (ro.operationType)
						{
						case RenderOperation::OT_POINT_LIST:
							primType = GL_POINTS;
							break;
						case RenderOperation::OT_LINE_LIST:
							primType = GL_LINES;
							break;
						case RenderOperation::OT_LINE_STRIP:
							primType = GL_LINE_STRIP;
							break;
						default:
						case RenderOperation::OT_TRIANGLE_LIST:
							primType = GL_TRIANGLES;
							break;
						case RenderOperation::OT_TRIANGLE_STRIP:
							primType = GL_TRIANGLE_STRIP;
							break;
						case RenderOperation::OT_TRIANGLE_FAN:
							primType = GL_TRIANGLE_FAN;
							break;
						}

						if (ro.useIndexes)
						{
							GLuint bufferId = static_cast<GLES2HardwareIndexBuffer*>(ro.indexData->indexBuffer.get())->getGLBufferId();
							//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);

							pBufferData =((char *)NULL + (ro.indexData->indexStart *
								ro.indexData->indexBuffer->getIndexSize())) ;

							GLenum indexType = (ro.indexData->indexBuffer->getType() == HardwareIndexBuffer::IT_16BIT) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
				
							CHECK_GL_ERROR_DEBUG();;
							GLenum primitive =  primType ;
							//glDrawElements(primitive, ro.indexData->indexCount, indexType, pBufferData);
							CHECK_GL_ERROR_DEBUG();;


		
							programstate->applyGLProgram(*((cocos2d::Mat4*)mTempXform));
							programstate->applyUniforms();

				
							programstate->applyAttributes();
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  bufferId);
							glDrawElements(primitive, ro.indexData->indexCount, indexType,pBufferData);
							CHECK_GL_ERROR_DEBUG();
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

							CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
							CHECK_GL_ERROR_DEBUG();
							// Update derived depth bias

						}
					}
					rend->postRender(this, mDestRenderSystem);
				}

			 // !skipBecauseOfLightType

			}
		}
	}*/
