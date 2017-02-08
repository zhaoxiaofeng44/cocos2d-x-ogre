/****************************************************************************
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

#include "GroupNode3D.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCGroupCommand.h"
#include "renderer/CCGLProgramState.h"
#include "base/CCDirector.h"

using namespace cocos2d;

GroupNode3D *GroupNode3D::create()
{
    GroupNode3D *ref = new (std::nothrow) GroupNode3D();
    if (ref && ref->init())
    {
        ref->autorelease();
        return ref;
    }
    CC_SAFE_DELETE(ref);
    return nullptr;
}

GroupNode3D::GroupNode3D()
: _groupCommand(nullptr)
{
}

GroupNode3D::~GroupNode3D()
{
    CC_SAFE_DELETE(_groupCommand);
}



void GroupNode3D::addChild(Node *child)
{
    CCASSERT( child != nullptr, "Argument must be non-nil");
	this->addChild(child, child->getLocalZOrder(), child->getName());
}

void GroupNode3D::addChild(Node *child, int zOrder)
{
    CCASSERT( child != nullptr, "Argument must be non-nil");
    this->addChild(child, zOrder, child->getName());
}

void GroupNode3D::addChild(Node *child, int zOrder, int tag)
{
    Node::addChild(child, zOrder, tag);

    if (_groupCommand == nullptr)
    {
        _groupCommand = new (std::nothrow) GroupCommand();
    }
}

void GroupNode3D::addChild(cocos2d::Node *child, int zOrder, const std::string &name)
{
    Node::addChild(child, zOrder, name);

	if (_groupCommand == nullptr)
    {
        _groupCommand = new (std::nothrow) GroupCommand();
    }
}

void GroupNode3D::removeChild(Node* child, bool cleanup)
{
    Node::removeChild(child, cleanup);
}

void GroupNode3D::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    // quick return if not visible. children won't be drawn.
    if (!_visible || !isVisitableByVisitingCamera())
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

    sortAllChildren();
    draw(renderer, _modelViewTransform, flags);

    // FIX ME: Why need to set _orderOfArrival to 0??
    // Please refer to https://github.com/cocos2d/cocos2d-x/pull/6920
    // setOrderOfArrival(0);

    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GroupNode3D::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if (_children.empty())
    {
        return;
    }

	generateGroupCommand();
	for(auto object : _children)
    {
        ((Node *)object)->visit(renderer, transform, flags);
    }
	renderer->popGroup();
}

void GroupNode3D::generateGroupCommand()
{
    Renderer* renderer = Director::getInstance()->getRenderer();
    _groupCommand->init(_globalZOrder);
    renderer->addCommand(_groupCommand);

    renderer->pushGroup(_groupCommand->getRenderQueueID());
}
