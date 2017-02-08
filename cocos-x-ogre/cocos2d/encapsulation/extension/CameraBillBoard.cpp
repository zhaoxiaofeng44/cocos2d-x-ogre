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

#include "CameraBillBoard.h"

#include "base/CCDirector.h"
#include "2d/CCCamera.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCGLProgramCache.h"

NS_CC_BEGIN

void CameraBillBoard::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags)
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
	

	if (flags & FLAGS_DIRTY_MASK)
	{
		if(!_children.empty())
		{
			sortAllChildren();

			cocos2d::Quaternion rotation;
			for(auto it=_children.cbegin(); it != _children.cend(); ++it)
			{
				(*it)->setAdditionalTransform(nullptr);
	
				Vec2 anchorPointInPoints;
				anchorPointInPoints = (*it)->getAnchorPointInPoints();

				Vec2 anchorPoint;
				anchorPoint.x = anchorPointInPoints.x * _scaleX;
				anchorPoint.y = anchorPointInPoints.y * _scaleY;

				Mat4 additional = (*it)->getNodeToWorldTransform();
				additional.getRotation(&rotation);
				rotation.inverse();
				Mat4 moveRotation = Mat4::IDENTITY;
				moveRotation.translate(anchorPoint.x,anchorPoint.y,0);
					
				Mat4 cameraRotation;
				Mat4::createRotation(rotation,&cameraRotation);
				moveRotation = moveRotation * cameraRotation;

				moveRotation.translate(-anchorPoint.x,-anchorPoint.y,0);

				(*it)->setAdditionalTransform(&moveRotation);
				(*it)->visit(renderer, _modelViewTransform, flags);
			}
		}
	}
	else
	{
		if(!_children.empty())
		{
			sortAllChildren();
			for(auto it=_children.cbegin(); it != _children.cend(); ++it)
				(*it)->visit(renderer, _modelViewTransform, flags);
		}
	}
	
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
    // FIX ME: Why need to set _orderOfArrival to 0??
    // Please refer to https://github.com/cocos2d/cocos2d-x/pull/6920
    // reset for next frame
    // _orderOfArrival = 0;
}

NS_CC_END