#include "OgreNodeProtocol.h"

#include <Ogre.h>
#include "components\OgreBridgeBufferManager.h"
#include "components\OgreBridgeTextureManager.h"
#include "OgreOgreManager.h"

USING_NS_CC;

namespace Ogre
{
	RenderState::RenderState()
	{
		_visible = false;
		glProgramState = NULL;
		mat4 = cocos2d::Mat4::IDENTITY;
	}

	RenderState::RenderState(RenderState& state)
	{
		_visible = state._visible;

		isTransparent = state.isTransparent;
		isDepthTestEnabled = state.isDepthTestEnabled;
		isDepthWriteEnabled = state.isDepthWriteEnabled;

		textureId = state.textureId;
		vertexBufferId = state.vertexBufferId;

		indexBufferId = state.indexBufferId;
		indexBufferType = state.indexBufferType;

		indexCount = state.indexCount;
		operationType = state.operationType;

		mat4 = state.mat4;
		blend = state.blend;
		meshCommand = state.meshCommand;
		glProgramState = state.glProgramState;
	}

	RenderState::RenderState(const RenderState& state)
	{
		_visible = state._visible;

		isTransparent = state.isTransparent;
		isDepthTestEnabled = state.isDepthTestEnabled;
		isDepthWriteEnabled = state.isDepthWriteEnabled;

		textureId = state.textureId;
		vertexBufferId = state.vertexBufferId;

		indexBufferId = state.indexBufferId;
		indexBufferType = state.indexBufferType;

		indexCount = state.indexCount;
		operationType = state.operationType;

		mat4 = state.mat4;
		blend = state.blend;
		meshCommand = state.meshCommand;
		glProgramState = state.glProgramState;
	}

	/*class QueueVisitor : public RenderQueue
	{
	public:

		virtual ~QueueVisitor(){};
		
		QueueVisitor():mCurProtocol(NULL){}
	
		void _visitor(Ogre::SceneNode *node)
		{
		
			SceneNode::ObjectIterator objs = node->getAttachedObjectIterator();
			while (objs.hasMoreElements())
			{
				MovableObject* mobj = objs.getNext();
				mobj->_updateRenderQueue(this);
			}

			SceneNode::ChildNodeIterator children =  node->getChildIterator();
			while (children.hasMoreElements())
			{
				this->_visitor(static_cast<SceneNode*>(children.getNext()));
			}
		}

		void visitor(cocos2d::NodeProtocol *protocol)
		{
			mCurProtocol = protocol;
			protocol->_update(true,true);
			_visitor(protocol);
			protocol->_update(true,true);
			mCurProtocol = NULL;
		}

		virtual void addRenderable(Renderable* pRend, uint8 groupID, ushort priority)
		{
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
		

			bool isTransparent = false;
			if (pTech->isTransparentSortingForced() || 
				(pTech->isTransparent() && 
				(!pTech->isDepthWriteEnabled() ||
					!pTech->isDepthCheckEnabled() ||
					pTech->hasColourWriteDisabled())))
			{
				if (pTech->isTransparentSortingEnabled())
					isTransparent = true;
			}
			else
			{
				//Technique::PassIterator pi = pTech->getPassIterator();
				//
				//while (pi.hasMoreElements())
				//{
				//	_render(pRend, pi.getNext(),false);
				//}
			}

			Technique::PassIterator pi = pTech->getPassIterator();

			while (pi.hasMoreElements())
			{
				_render(pRend, pi.getNext(),isTransparent);
			}
		}
	
		void _render(Renderable *rend,const Pass *pass,bool isTransport)
		{
			MeshStateMap &stateMap = mCurProtocol->getRenderStates(); 
			MeshStateMap::iterator stateItor = stateMap.find(rend);

			RenderState *renderState;
			Ogre::RenderOperation ro;
			rend->preRender(OgreManager::getInstance()->getSceneManager(), OgreManager::getInstance()->getRenderSystem());
			rend->getRenderOperation(ro);

			if(stateItor != stateMap.end())
			{
				renderState = &stateItor->second;

				_refreshVectexBufferId(ro,renderState->vertexBufferId);
			}
			else
			{
				stateMap[rend] = RenderState();
				stateItor = stateMap.find(rend); 

				renderState = &stateItor->second;

				    //create and set our custom shader
			    auto shader =GLProgram::createWithFilenames("cylinder.vert","cylinder.frag");
				renderState->glProgramState = GLProgramState::create(shader);
				CC_SAFE_RETAIN(renderState->glProgramState);
				 // cylinder->setGLProgramState(_state);

				//auto glProgram =cocos2d::GLProgramCache::getInstance()->getGLProgram();
				//renderState->glProgramState = cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
		
				_refreshProgramState(ro,renderState->glProgramState,renderState->vertexBufferId);
			}

			renderState->blend.src = pass->getSourceBlendFactor();
			renderState->blend.dst = pass->getDestBlendFactor();

			int tempCount = rend->getNumWorldTransforms();
			if(tempCount > 0)
			{
				rend->getWorldTransforms(mTempXform);
				renderState->mat4 =  *((cocos2d::Mat4*)mTempXform);
			}
			
			// Reissue any texture gen settings which are dependent on view matrix
			Pass::ConstTextureUnitStateIterator texIter = pass->getTextureUnitStateIterator();
			size_t unit = 0;
			while (texIter.hasMoreElements())
			{
				TextureUnitState* pTex = texIter.getNext();
				pTex->_prepare();
				//if (pTex->hasViewRelativeTextureCoordinateGeneration())
				//{
				//	mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
				//}
				//++unit;
				BridgeTexturePtr textP = BridgeTexturePtr(pTex->_getTexturePtr());
				if(!textP.isNull())
				{
					renderState->textureId = textP->getGLID();
				}
			}

			if (ro.useIndexes)
			{
				renderState->_visible = true;

				renderState->indexBufferId = static_cast<BridgeHardwareIndexBuffer*>(ro.indexData->indexBuffer.get())->getGLBufferId();
			
				renderState->indexBufferType = ro.indexData->indexBuffer->getType();

				renderState->operationType = ro.operationType;
				renderState->indexCount = ro.indexData->indexCount;
		
				renderState->isTransparent = isTransport;
				renderState->isDepthTestEnabled = pass->getDepthCheckEnabled();
				renderState->isDepthWriteEnabled = pass->getDepthWriteEnabled();
			}
		}
	
		void _refreshVectexBufferId(RenderOperation &ro,GLuint &bufferId)
		{
			void* pBufferData = 0;
			const VertexDeclaration::VertexElementList& decl =
				ro.vertexData->vertexDeclaration->getElements();
			VertexDeclaration::VertexElementList::const_iterator elem,elemEnd = decl.end();

			for (elem = decl.begin(); elem != elemEnd; ++elem)
			{
				if (!ro.vertexData->vertexBufferBinding->isBufferBound(elem->getSource()))
					continue; // skip unbound elements
				CHECK_GL_ERROR_DEBUG();

				HardwareVertexBufferSharedPtr vertexBuffer =
					ro.vertexData->vertexBufferBinding->getBuffer(elem->getSource());

				bufferId = static_cast<const BridgeHardwareVertexBuffer*>(vertexBuffer.get())->getGLBufferId();
			}
		}

		void _refreshProgramState(RenderOperation &ro,cocos2d::GLProgramState *state,GLuint &bufferId)
		{
			void* pBufferData = 0;
			const VertexDeclaration::VertexElementList& decl =
				ro.vertexData->vertexDeclaration->getElements();
			VertexDeclaration::VertexElementList::const_iterator elem, elemEnd;
			elemEnd = decl.end();

			for (elem = decl.begin(); elem != elemEnd; ++elem)
			{
				if (!ro.vertexData->vertexBufferBinding->isBufferBound(elem->getSource()))
					continue; // skip unbound elements
				CHECK_GL_ERROR_DEBUG();

				HardwareVertexBufferSharedPtr vertexBuffer =
					ro.vertexData->vertexBufferBinding->getBuffer(elem->getSource());

				bufferId = static_cast<const BridgeHardwareVertexBuffer*>(vertexBuffer.get())->getGLBufferId();
		
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
					attribute_name = "a_position";GLProgram::ATTRIBUTE_NAME_POSITION;
					break;
				case VES_BLEND_WEIGHTS:
					attribute_name = GLProgram::ATTRIBUTE_NAME_BLEND_WEIGHT;
					break;
				case VES_BLEND_INDICES:
					attribute_name = GLProgram::ATTRIBUTE_NAME_BLEND_INDEX;
					break;
				case VES_NORMAL:
					attribute_name = "a_color"; GLProgram::ATTRIBUTE_NAME_NORMAL;
					break;
				case VES_DIFFUSE:
					attribute_name = GLProgram::ATTRIBUTE_NAME_COLOR;
					break;
				//case VES_SPECULAR:
				//	break;
				case VES_TEXTURE_COORDINATES:
					attribute_name = "a_texCoord";GLProgram::ATTRIBUTE_NAME_TEX_COORD;
					break;
				//case VES_BINORMAL:
				//	break;
				//case VES_TANGENT:
				//	break;
				}

				//int type = GLES2HardwareBufferManager::getGLType(elem->getType());
				GLsizei stride = static_cast<GLsizei>(vertexBuffer->getVertexSize());
			
				state->setVertexAttribPointer(attribute_name,
					size,
					type,
					normalised,
					stride,
					(GLvoid*)pBufferData
				);
			}
		}

	protected:

		Ogre::Matrix4 mTempXform[256];

		bool mVisibleFlag;
		MeshStateMap mMeshStates;
	
		cocos2d::NodeProtocol *mCurProtocol;
	};*/
}
/*
static Ogre::QueueVisitor *s_QueueVisitor = NULL;


NS_CC_BEGIN

NodeProtocol::NodeProtocol(Ogre::SceneManager * creator) :
Ogre::SceneNode(NULL)
{
	if(NULL == s_QueueVisitor)   //cannot crate this object before root
	{
		s_QueueVisitor = new Ogre::QueueVisitor();
	}
}

NodeProtocol::NodeProtocol(Ogre::SceneManager * creator,const std::string &name) :
	Ogre::SceneNode(NULL)
{
	if(NULL == s_QueueVisitor)   //cannot crate this object before root
	{
		s_QueueVisitor = new Ogre::QueueVisitor();
	}
}

NodeProtocol::~NodeProtocol()
{
}

void NodeProtocol::setCocosMatrix4(const cocos2d::Mat4 &mat)
{
	cocos2d::Quaternion rotation;
	cocos2d::Vec3 scale, translation;

	mat.decompose(&scale, &rotation, &translation);

	/*mCocosScale.x = scale.x;
	mCocosScale.y = scale.y;
	mCocosScale.z = scale.z;

	mCocosPosition.x = translation.x;
	mCocosPosition.y = translation.y;
	mCocosPosition.z = translation.z;

	mCocosQuaternion.w = rotation.w;
	mCocosQuaternion.x = rotation.x;
	mCocosQuaternion.y = rotation.y;
	mCocosQuaternion.z = rotation.z;*/

	/*mDerivedScale.x = scale.x;
	mDerivedScale.y = scale.y;
	mDerivedScale.z = scale.z;

	mDerivedPosition.x = translation.x;
	mDerivedPosition.y = translation.y;
	mDerivedPosition.z = translation.z;

	mDerivedOrientation.w = rotation.w;
	mDerivedOrientation.x = rotation.x;
	mDerivedOrientation.y = rotation.y;
	mDerivedOrientation.z = rotation.z;

	//needUpdate();
}

MeshStateMap &NodeProtocol::getRenderStates()
{
	return mRenderStateMap;
}

void NodeProtocol::updateRenderQueue()
{
	s_QueueVisitor->visitor(this);
}

const Ogre::Vector3 & NodeProtocol::_getDerivedScale(void) const
{
	return mCocosScale;
}

const Ogre::Vector3 & NodeProtocol::_getDerivedPosition(void) const
{
	return mCocosPosition;
}

const Ogre::Quaternion & NodeProtocol::_getDerivedOrientation(void) const
{
	return mCocosQuaternion;
}

Ogre::Node* NodeProtocol::createChildImpl(void)
{
	return new NodeProtocol(NULL);
}

Ogre::Node* NodeProtocol::createChildImpl(const Ogre::String& name)
{
	return createChildImpl();
}

NS_CC_END*/