#ifndef __CC_Node_Protocol_H__
#define __CC_Node_Protocol_H__


#include <cocos2d.h>
#include <OgreSceneNode.h>

NS_CC_BEGIN
	class NodeProtocol;
NS_CC_END

namespace Ogre
{
	class Node;
	class QueueVisitor;

	struct RenderState
	{
	public:
		
		bool _visible;
		bool isTransparent;
		bool isDepthTestEnabled;
		bool isDepthWriteEnabled;

		GLuint textureId;	
		GLuint vertexBufferId;
	
		GLuint indexBufferId;
		GLuint indexBufferType;
	
		GLuint indexCount;
		GLuint operationType;

		cocos2d::Mat4 mat4;
		cocos2d::BlendFunc blend;
		cocos2d::MeshCommand meshCommand;
		cocos2d::GLProgramState *glProgramState;

		RenderState();

		RenderState(RenderState&);

		RenderState(const RenderState&);
	};
}

typedef std::vector<Ogre::RenderState> OgreRenderStateVector;

typedef std::vector<cocos2d::MeshCommand *> MeshCommandVector;
typedef std::vector<cocos2d::GLProgramState *> ProgramStateVector;

typedef std::unordered_map<const Ogre::Renderable *,Ogre::RenderState> MeshStateMap;

NS_CC_BEGIN
	/*
class NodeProtocol :
	public Ogre::SceneNode
{
public:

	NodeProtocol(Ogre::SceneManager * creator);

	NodeProtocol(Ogre::SceneManager * creator,const std::string &name);

	virtual ~NodeProtocol();

	void setCocosMatrix4(const cocos2d::Mat4 &);

	MeshStateMap &getRenderStates();

	void updateRenderQueue();

	virtual Ogre::Node* createChildImpl(void) override;

	virtual Ogre::Node* createChildImpl(const Ogre::String & name) override;

	*virtual const Ogre::Vector3 & _getDerivedScale(void) const override;

	virtual const Ogre::Vector3 & _getDerivedPosition(void) const override;

	virtual const Ogre::Quaternion & _getDerivedOrientation(void) const override;*

protected:

	Ogre::Vector3 mCocosScale;
	Ogre::Vector3 mCocosPosition;
	Ogre::Quaternion mCocosQuaternion;

	MeshStateMap mRenderStateMap;
};*/

NS_CC_END
#endif // __CC_Node_Protocol_H__
