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

#include "OgreOgreSprite3D.h"


#include <Ogre.h>
#include "OgreOgreManager.h"
#include "OgreNodeProtocol.h"
#include <OgreTechnique.h>
#include <OgreMovableObject.h>
#include <OgreMaterialManager.h>
#include <OgreEntity.h>

#include "components\OgreBridgeBufferManager.h"
#include "components\OgreBridgeTextureManager.h"
#include "OgreOgreManager.h"
#include "OgreSceneNode.h"

USING_NS_CC;


static const char* SHADER_OGRE_POSITION_TEXTURE			= "SHADER_OGRE_POSITION_TEXTURE";
static const char* SHADER_OGRE_POSITION_TEXTURE_COLOR	= "SHADER_OGRE_POSITION_TEXTURE_COLOR";

namespace Ogre
{

	static void covertInverse(cocos2d::Mat4 &dst,Ogre::Matrix4 &src)
	{
		dst.m[0] = src[0][0];dst.m[1] = src[1][0];dst.m[2] = src[2][0];dst.m[3] = src[3][0];
		dst.m[4] = src[0][1];dst.m[5] = src[1][1];dst.m[6] = src[2][1];dst.m[7] = src[3][1];
		dst.m[8] = src[0][2];dst.m[9] = src[1][2];dst.m[10] = src[2][2];dst.m[11] = src[3][2];
		dst.m[12] = src[0][3];dst.m[13] = src[1][3];dst.m[14] = src[2][3];dst.m[15] = src[3][3];
	}

	static cocos2d::GLProgramState *getOrCreateWithGLProgramName(const std::string &name)
	{
		cocos2d::GLProgram *glProgram = cocos2d::GLProgramCache::getInstance()->getProgram(name);
		if (!glProgram)
		{
			if (name == SHADER_OGRE_POSITION_TEXTURE )
			{
				glProgram = GLProgram::createWithFilenames("shader/ccShader_PositionTex.vert","shader/ccShader_PositionTex.frag");
				cocos2d::GLProgramCache::getInstance()->addGLProgram(glProgram,SHADER_OGRE_POSITION_TEXTURE);
		
			}
			else if(name == SHADER_OGRE_POSITION_TEXTURE_COLOR )
			{
				glProgram = GLProgram::createWithFilenames("shader/ccShader_PositionTexColor.vert","shader/ccShader_PositionTexColor.frag");
				cocos2d::GLProgramCache::getInstance()->addGLProgram(glProgram,SHADER_OGRE_POSITION_TEXTURE_COLOR);
			}
		}
		return cocos2d::GLProgramState::getOrCreateWithGLProgram(glProgram);
	}
	
	enum {
		OGRE_VERTEX_ATTRIB_FLAG_NONE	     = 0,
		OGRE_VERTEX_ATTRIB_FLAG_POSITION     = 1 << 0,
		OGRE_VERTEX_ATTRIB_FLAG_COLOR        = 1 << 1,
		OGRE_VERTEX_ATTRIB_FLAG_TEX_COORD    = 1 << 2,
		OGRE_VERTEX_ATTRIB_FLAG_NORMAL       = 1 << 3,
		OGRE_VERTEX_ATTRIB_FLAG_BLEND_WEIGHT = 1 << 4,
		OGRE_VERTEX_ATTRIB_FLAG_BLEND_INDEX  = 1 << 5,

		OGRE_UNIFORM_FLAG_COLOR				 = 1 << 6,

		OGRE_PROGRAM_FLAG_POS_TEX = (OGRE_VERTEX_ATTRIB_FLAG_POSITION | OGRE_VERTEX_ATTRIB_FLAG_TEX_COORD),
		OGRE_PROGRAM_FLAG_POS_TEX_NORMAL = (OGRE_VERTEX_ATTRIB_FLAG_POSITION | OGRE_VERTEX_ATTRIB_FLAG_TEX_COORD | OGRE_VERTEX_ATTRIB_FLAG_NORMAL),
		OGRE_PROGRAM_FLAG_POS_COLOR_TEX = (OGRE_VERTEX_ATTRIB_FLAG_POSITION | OGRE_VERTEX_ATTRIB_FLAG_COLOR | OGRE_VERTEX_ATTRIB_FLAG_TEX_COORD),
		OGRE_PROGRAM_FLAG_POS_COLOR_TEX_NORMAL = (OGRE_VERTEX_ATTRIB_FLAG_POSITION | OGRE_VERTEX_ATTRIB_FLAG_COLOR | OGRE_VERTEX_ATTRIB_FLAG_TEX_COORD | OGRE_VERTEX_ATTRIB_FLAG_NORMAL),
	};

	static cocos2d::GLProgramState * getOrCreateWithVertexFlag(const int &flag)
	{
		cocos2d::GLProgramState *glState;
		switch(flag)
		{
		case OGRE_PROGRAM_FLAG_POS_TEX:
		case OGRE_PROGRAM_FLAG_POS_TEX_NORMAL:

			glState = getOrCreateWithGLProgramName(SHADER_OGRE_POSITION_TEXTURE);
			break;
	
		case OGRE_PROGRAM_FLAG_POS_COLOR_TEX:
		case OGRE_PROGRAM_FLAG_POS_COLOR_TEX_NORMAL:
		default:
			glState = getOrCreateWithGLProgramName(SHADER_OGRE_POSITION_TEXTURE_COLOR);
			
			break;
		}
		return glState;
	}

	const int & getVertexAttribFlagBySemantic(VertexElementSemantic semantic)
	{
		switch(semantic)
		{
		case VES_POSITION:
				return OGRE_VERTEX_ATTRIB_FLAG_POSITION;
			break;
		case VES_DIFFUSE:
				return OGRE_VERTEX_ATTRIB_FLAG_COLOR;
			break;
		case VES_TEXTURE_COORDINATES:
				return OGRE_VERTEX_ATTRIB_FLAG_TEX_COORD;
			break;
		case VES_NORMAL:
				return OGRE_VERTEX_ATTRIB_FLAG_NORMAL;
			break;
		case VES_BLEND_WEIGHTS:
				return OGRE_VERTEX_ATTRIB_FLAG_BLEND_WEIGHT;
			break;
		case VES_BLEND_INDICES:
				return OGRE_VERTEX_ATTRIB_FLAG_BLEND_INDEX;
			break;
		default:
				return OGRE_VERTEX_ATTRIB_FLAG_NONE;
			break;
		}
	}

	const char* getVertexAttribNameBySemantic(VertexElementSemantic semantic)
	{
		const char* attribute_name;
		switch(semantic)
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
			attribute_name =  GLProgram::ATTRIBUTE_NAME_NORMAL;
			break;
		case VES_DIFFUSE:
			attribute_name = GLProgram::ATTRIBUTE_NAME_COLOR;
			break;
		case VES_TEXTURE_COORDINATES:
			attribute_name = GLProgram::ATTRIBUTE_NAME_TEX_COORD;
			break;
		//case VES_SPECULAR:VES_BINORMAL:VES_TANGENT:
		//	break;
		}
		return attribute_name;
	}
	
	void refreshVertexAttribTypeByType(VertexElementType elementType,GLint &size, GLenum &type,GLboolean &normalized)
	{
		switch(elementType)
		{
			case VET_FLOAT1:
				type =  GL_FLOAT;
				size = 1;
				normalized = GL_FALSE;
				break;
			case VET_FLOAT2:
				type =  GL_FLOAT;
				size = 2;
				normalized = GL_FALSE;
				break;
			case VET_FLOAT3:
				type =  GL_FLOAT;
				size = 3;
				normalized = GL_FALSE;
				break;
			case VET_FLOAT4:
				type =  GL_FLOAT;
				size = 4;
				normalized = GL_FALSE;
				break;
			case VET_SHORT1:
				type =  GL_SHORT;
				size = 1;
				normalized = GL_FALSE;
				break;
			case VET_SHORT2:
				type =  GL_SHORT;
				size = 2;
				normalized = GL_FALSE;
				break;
			case VET_SHORT3:
				type =  GL_SHORT;
				size = 3;
				normalized = GL_FALSE;
				break;
			case VET_SHORT4:
				type =  GL_SHORT;
				size = 4;
				normalized = GL_FALSE;
				break;
			case VET_COLOUR:
			case VET_COLOUR_ABGR:
			case VET_COLOUR_ARGB:
			case VET_UBYTE4:
				size = 4;
				normalized = GL_TRUE;
				type =  GL_UNSIGNED_BYTE;
				break;
			default:
				break;
		};
	}

	class QueueVisitor : public RenderQueue
	{
	public:

		virtual ~QueueVisitor(){};
		
		QueueVisitor(){}
	
		void setSceneManager(Ogre::SceneManager *manager)
		{
			mSceneManager = manager; 
		}

		void setRenderSystem(Ogre::RenderSystem *system)
		{
			mRenderSystem = system;
		}

		void _visitor(Ogre::SceneNode *node)
		{
			SceneNode::ObjectIterator objs = node->getAttachedObjectIterator();
			while (objs.hasMoreElements())
			{
				MovableObject* mobj = objs.getNext();
				if(mobj && mobj->getVisible())
				{
					mobj->_notifyCurrentCamera( OgreManager::getInstance()->getCamera());
					mobj->_updateRenderQueue(this);
				}	
			}
			SceneNode::ChildNodeIterator children =  node->getChildIterator();
			while (children.hasMoreElements())
			{
				this->_visitor(static_cast<SceneNode*>(children.getNext()));
			}
		}

		void visitor(Ogre::SceneNode *protocol,MeshStateMap *stateMap)
		{
			_tempStateMap = stateMap;
			protocol->_update(true,true);
			_visitor(protocol);
			_tempStateMap = NULL;
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
			
			Technique::PassIterator pi = pTech->getPassIterator();

			while (pi.hasMoreElements())
			{
				const Pass * pass = pi.getNext();
				if (pRend->preRender(mSceneManager,mRenderSystem))
				{
					_render(pRend, pass, isTransparent);
				}		
				pRend->postRender(mSceneManager,mRenderSystem);
			}
		}
	
		void _render(Renderable *rend,const Pass *pass,bool isTransport)
		{
			MeshStateMap::iterator stateItor = _tempStateMap->find(rend);

			RenderState *renderState;
			HardwareBuffer *hardwareBuffer;

			rend->getRenderOperation(mOperation);

			if(stateItor != _tempStateMap->end())
			{
				renderState = &stateItor->second;
				_refreshVertexAttribPointer(mOperation,&renderState->glProgramState,&hardwareBuffer);
			}
			else
			{
				(*_tempStateMap)[rend] = RenderState();
				stateItor = _tempStateMap->find(rend); 

				renderState = &stateItor->second;
				_formatVertexBuffer(mOperation,&renderState->glProgramState,&hardwareBuffer);
			}

			renderState->vertexBufferId = static_cast<const BridgeHardwareVertexBuffer*>(hardwareBuffer)->getGLBufferId();
			renderState->blend.src = pass->getSourceBlendFactor();
			renderState->blend.dst = pass->getDestBlendFactor();

			int matCount = rend->getNumWorldTransforms();
			if(matCount > 0)
			{
				rend->getWorldTransforms(mMatrix4);
				
				for(int i= 0 ;i < matCount; i++)
				{
					covertInverse(renderState->mat4,mMatrix4[i]);
				}
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

			if (mOperation.useIndexes)
			{
				renderState->_visible = true;

				renderState->indexBufferId = static_cast<BridgeHardwareIndexBuffer*>(mOperation.indexData->indexBuffer.get())->getGLBufferId();
			
				renderState->indexBufferType = mOperation.indexData->indexBuffer->getType();

				renderState->operationType = mOperation.operationType;
				renderState->indexCount = mOperation.indexData->indexCount;
		
				renderState->isTransparent = isTransport;
				renderState->isDepthTestEnabled = pass->getDepthCheckEnabled();
				renderState->isDepthWriteEnabled = pass->getDepthWriteEnabled();
			}
		}
	
		void _refreshVertexAttribPointer(RenderOperation &ro,cocos2d::GLProgramState **glProgramState,HardwareBuffer **vertextBuffer)
		{
			*vertextBuffer = nullptr;

			const VertexDeclaration::VertexElementList& decl =
				ro.vertexData->vertexDeclaration->getElements();
			VertexDeclaration::VertexElementList::const_iterator elem,elemEnd = decl.end();

			HardwareVertexBufferSharedPtr vertexBufferPtr;

			GLint size; 
			GLenum type;
			GLboolean normalized; 
			for (elem = decl.begin(); elem != elemEnd; ++elem)
			{
				if (ro.vertexData->vertexBufferBinding->isBufferBound(elem->getSource()))
				{
					vertexBufferPtr = ro.vertexData->vertexBufferBinding->getBuffer(elem->getSource());

					if(nullptr != *vertextBuffer && *vertextBuffer != vertexBufferPtr.get())
						cocos2d::log("ERROR: OgreOgreSprite3D cannot multiple vertexBuffer!");
					else
						*vertextBuffer = vertexBufferPtr.get();

					refreshVertexAttribTypeByType(elem->getType(),size,type,normalized);
					GLsizei stride = static_cast<GLsizei>(vertexBufferPtr->getVertexSize());
					(*glProgramState)->setVertexAttribPointer(getVertexAttribNameBySemantic(elem->getSemantic()),
						size,
						type,
						normalized,
						stride,
						(GLvoid*)((char *)NULL + (elem->getOffset()))
					);
				} 
			}
		}

		void _formatVertexBuffer(RenderOperation &ro,cocos2d::GLProgramState **glProgramState,HardwareBuffer **vertextBuffer)
		{
			*vertextBuffer = nullptr;
			*glProgramState = nullptr;

			const VertexDeclaration::VertexElementList& decl =
				ro.vertexData->vertexDeclaration->getElements();
			VertexDeclaration::VertexElementList::const_iterator elem,elemEnd = decl.end();

			int flag = OGRE_VERTEX_ATTRIB_FLAG_NONE;
			for (elem = decl.begin(); elem != elemEnd; ++elem)
			{
				if (ro.vertexData->vertexBufferBinding->isBufferBound(elem->getSource()))
				{
					flag = flag | getVertexAttribFlagBySemantic(elem->getSemantic());
				}
			}
			*glProgramState = getOrCreateWithVertexFlag(flag);
			_refreshVertexAttribPointer(ro,glProgramState,vertextBuffer);
		}

	protected:

		Ogre::Matrix4 mMatrix4[256];
		Ogre::RenderOperation mOperation;

		MeshStateMap *_tempStateMap;

		Ogre::SceneManager *mSceneManager;
		Ogre::RenderSystem *mRenderSystem;
	};
}

static Ogre::QueueVisitor *s_QueueVisitor = NULL;


NS_CC_BEGIN


GLuint vertexBuffer;
GLuint indexBuffer;

typedef struct {
	GLfloat cPosition[3];
	GLfloat cColor[4];
	GLfloat cTexCoord[2];
} Vertex;



OgreSprite3D* OgreSprite3D::create()
{
	auto node = new OgreSprite3D();
	if (node && node->init())
	{
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return nullptr;
}

OgreSprite3D* OgreSprite3D::create(const std::string &filePath)
{
	auto node = new OgreSprite3D();
	if (node && node->initWithFile(filePath))
	{
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return nullptr;
}


OgreSprite3D::OgreSprite3D() :
	_blend(cocos2d::BlendFunc::ALPHA_NON_PREMULTIPLIED)
{
	_nodeProtocol = OgreManager::getInstance()->getSceneManager()->createSceneNode();
}

OgreSprite3D::~OgreSprite3D()
{
	CC_SAFE_DELETE(_nodeProtocol);
}

bool OgreSprite3D::init()
{
	if(NULL == s_QueueVisitor)   //cannot crate this object before root
	{
		s_QueueVisitor = new Ogre::QueueVisitor();
		
		s_QueueVisitor->setSceneManager(OgreManager::getInstance()->getSceneManager());
		s_QueueVisitor->setRenderSystem(OgreManager::getInstance()->getRenderSystem());
	}
	/*_texture = Director::getInstance()->getTextureCache()->addImage("HelloWorld.png");
	Size visibleSize = Director::getInstance()->getVisibleSize() * 0.3;
    Vec2 origin = Director::getInstance()->getVisibleOrigin() * 0.3;

    /////////////////////////////
	this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

	_texture = Director::getInstance()->getTextureCache()->addImage("HelloWorld.png");

	Vertex data[] =
	{
		{ { origin.x, origin.y, 0 }, { 1, 1, 1, 1 }, { 0, 1 } },
		{ { visibleSize.width , origin.y, 0 }, { 1, 1, 1, 1 }, { 1, 1 } },
		{ { origin.x, visibleSize.height, 0 }, { 1, 1, 1, 1 }, { 0, 0 } },
		{ { visibleSize.height, visibleSize.width, 0 }, { 1, 1, 1, 1 }, { 1, 0 } }
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);


	GLubyte Indices[] = { 0, 1, 2, 2, 3, 1 };

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

	return CCNode::init();
}

bool OgreSprite3D::initWithFile( const std::string &filePath)
{
	auto entry = OgreManager::getInstance()->createEntity(filePath);
	if (entry && OgreSprite3D::init())
	{
		this->attachObject(entry);
		return true;
	}
	return false;
}

void OgreSprite3D::attachObject(Ogre::MovableObject *movable)
{
	if (_nodeProtocol)
		_nodeProtocol->attachObject(movable);
}

void OgreSprite3D::detachObject(Ogre::MovableObject *movable)
{
	if (_nodeProtocol)
		_nodeProtocol->detachObject(movable);
}

void OgreSprite3D::detachAllObjects()
{
	if (_nodeProtocol)
		_nodeProtocol->detachAllObjects();
}

void OgreSprite3D::updateParentTransform(const Mat4& parentTransform)
{
	_modelViewTransform = this->transform(parentTransform);

	if (_nodeProtocol)
	{
		_modelViewTransform.decompose(&mCosScale, &mCosRotation, &mCosTranslation);

		_nodeProtocol->setScale(Ogre::Vector3(mCosScale.x,mCosScale.y,mCosScale.z));
		_nodeProtocol->setPosition(Ogre::Vector3(mCosTranslation.x,mCosTranslation.y,mCosTranslation.z));
		_nodeProtocol->setOrientation(Ogre::Quaternion(mCosRotation.w,mCosRotation.x,mCosRotation.y,mCosRotation.z));

		_nodeProtocol->needUpdate();
	}
}

uint32_t OgreSprite3D::processParentFlags(const Mat4& parentTransform, uint32_t parentFlags)
{
	if (_usingNormalizedPosition) {
		CCASSERT(_parent, "setNormalizedPosition() doesn't work with orphan nodes");
		if ((parentFlags & FLAGS_CONTENT_SIZE_DIRTY) || _normalizedPositionDirty) {
			auto s = _parent->getContentSize();
			_position.x = _normalizedPosition.x * s.width;
			_position.y = _normalizedPosition.y * s.height;
			_transformUpdated = _transformDirty = _inverseDirty = true;
			_normalizedPositionDirty = false;
		}
	}

	uint32_t flags = parentFlags;
	flags |= (_transformUpdated ? FLAGS_TRANSFORM_DIRTY : 0);
	flags |= (_contentSizeDirty ? FLAGS_CONTENT_SIZE_DIRTY : 0);

	if (flags & FLAGS_DIRTY_MASK)
		updateParentTransform(parentTransform);

	_transformUpdated = false;
	_contentSizeDirty = false;

	return flags;
}

void OgreSprite3D::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
	// quick return if not visible. children won't be drawn.
	if (!_visible)
	{
		return;
	}

	uint32_t flags = processParentFlags(parentTransform, parentFlags);

	// IMPORTANT:
	// To ease the migration to v3.0, we still support the Mat4 stack,
	// but it is deprecated and your code should not rely on it
	Director* director = Director::getInstance();
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);

	bool visibleByCamera = isVisitableByVisitingCamera();
	if (visibleByCamera)
	{
		this->draw(renderer, _modelViewTransform, flags);
	}

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	// FIX ME: Why need to set _orderOfArrival to 0??
	// Please refer to https://github.com/cocos2d/cocos2d-x/pull/6920
	// reset for next frame
	// _orderOfArrival = 0;
}

void OgreSprite3D::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
	if(_nodeProtocol)
	{
		//glClearColor(1.0f,1.0f,0.0f,1.0f);
		s_QueueVisitor->visitor(_nodeProtocol,&_meshStateMap);

		Color4F color(getDisplayedColor());
		color.a = getDisplayedOpacity() / 255.0f;

		/*_customCommand.init(_globalZOrder);
		_customCommand.func = CC_CALLBACK_0(OgreSprite3D::onDraw, this, transform, flags);
		renderer->addCommand(&_customCommand);
		return ;*/

		MeshStateMap::iterator elem ,end = _meshStateMap.end();
			
		for( elem = _meshStateMap.begin(); elem != end; ++ elem)
		{
			Ogre::RenderState &state = elem->second;
			if(state._visible)
			{
				state._visible = false;
				MeshCommand  &meshCommand = state.meshCommand;
				//state.blend.src =  GL_SRC_ALPHA;
				//state.blend.dst.m =  GL_ONE_MINUS_DST_ALPHA;
				meshCommand.init(_globalZOrder,state.textureId,state.glProgramState,state.blend,state.vertexBufferId,state.indexBufferId,state.operationType,state.indexBufferType,state.indexCount,state.mat4);
				
			    //meshCommand.setDepthTestEnabled(true);
				//meshCommand.setTransparent(true);
				//meshCommand.setDepthWriteEnabled(false);
				meshCommand.setTransparent(state.isTransparent);
				meshCommand.setDepthTestEnabled(state.isDepthTestEnabled);
				meshCommand.setDepthWriteEnabled(state.isDepthWriteEnabled);

				meshCommand.setDisplayColorEnabled(false);
				meshCommand.genMaterialID(state.textureId, state.glProgramState, state.vertexBufferId, state.indexBufferId, state.blend);

				renderer->addCommand(&meshCommand);
			}	
		}
	}
}


//we call our actual opengl commands here
void OgreSprite3D::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{
	
	MeshStateMap::iterator elem ,end = _meshStateMap.end();
			
	for( elem = _meshStateMap.begin(); elem != end; ++ elem)
	{

		Ogre::RenderState &state = elem->second;
	

		auto s_shader = getGLProgram();
		s_shader->use();
		s_shader->setUniformsForBuiltins(transform);

		GL::bindTexture2D(state.textureId);


		glBindBuffer(GL_ARRAY_BUFFER,state.vertexBufferId);

		state.glProgramState->applyAttributes();
		/*glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, cPosition));

		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, cTexCoord));

		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, cColor));*/

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,state.indexBufferId);
		glDrawElements(state.operationType,state.indexCount,GL_UNSIGNED_BYTE, 0);

		CHECK_GL_ERROR_DEBUG();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	}
}


void OgreSprite3D::setBlendFunc(const cocos2d::BlendFunc &blendFunc)
{
	if (_blend.src != blendFunc.src || _blend.dst != blendFunc.dst)
	{
		_blend = blendFunc;
	}
}

const cocos2d::BlendFunc& OgreSprite3D::getBlendFunc() const
{
	return _blend;
}
NS_CC_END