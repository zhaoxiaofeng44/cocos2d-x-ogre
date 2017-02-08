/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.

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

#ifndef __CCGroupNode3D_H__
#define __CCGroupNode3D_H__

#include "2d/CCNode.h"

namespace cocos2d {
    class GroupCommand;
}

class GroupNode3D : public cocos2d::Node
{
public:
    static GroupNode3D *create();
public:
	/**
     * @js ctor
     */
    GroupNode3D();
    /**
     * @ js NA
     * @ lua NA
     */
    ~GroupNode3D();
    /**
     *  @js NA
     */
   

	virtual void addChild(Node *child) override;
	virtual void addChild(Node *child, int zOrder) override;
    virtual void addChild(cocos2d::Node *pChild, int zOrder, int tag) override;
    virtual void addChild(cocos2d::Node *pChild, int zOrder, const std::string &name) override;
    virtual void removeChild(cocos2d::Node* child, bool cleanup) override;
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override;
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    
protected:
    void generateGroupCommand();

    cocos2d::GroupCommand* _groupCommand;
};


#endif /*__CCGroupNode3D_H__*/
