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

#ifndef __CCOgreSprite3D_H__
#define __CCOgreSprite3D_H__

#include "cocos2d.h"
#include "OgreNodeProtocol.h"

namespace Ogre
{
	class SceneNode;
	class MovableObject;
}


NS_CC_BEGIN
class NodeProtocol;

/** particle univeise system */
class OgreSprite3D :
	public cocos2d::Node,
	public cocos2d::BlendProtocol
{
public:

	static OgreSprite3D* create();

	static OgreSprite3D* create(const std::string &filePath);

	virtual void setBlendFunc(const cocos2d::BlendFunc &blendFunc) override;

	virtual const cocos2d::BlendFunc &getBlendFunc() const override;

CC_CONSTRUCTOR_ACCESS:

	OgreSprite3D();
	
	virtual ~OgreSprite3D();

	virtual bool init();

	virtual bool initWithFile( const std::string &filePath);

	virtual void detachAllObjects();

	virtual void detachObject(Ogre::MovableObject *movable);

	virtual void attachObject(Ogre::MovableObject *movable);

	virtual void updateParentTransform(const Mat4& parentTransform);

	uint32_t processParentFlags(const Mat4& parentTransform, uint32_t parentFlags);

	virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;

	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;

	void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);

protected:

	cocos2d::BlendFunc _blend;
	
	MeshStateMap _meshStateMap;
	Ogre::SceneNode *_nodeProtocol;

	cocos2d::Quaternion mCosRotation;
	cocos2d::Vec3 mCosScale, mCosTranslation;

	cocos2d::CustomCommand _customCommand;

	cocos2d::Texture2D *_texture;

};
NS_CC_END
#endif // __ParticleSprite3D_H_
