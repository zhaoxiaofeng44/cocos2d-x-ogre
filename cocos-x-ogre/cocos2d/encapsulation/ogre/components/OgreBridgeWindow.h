/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2012 Torus Knot Software Ltd

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
-----------------------------------------------------------------------------
*/

#ifndef __BridgeWindow_H__
#define __BridgeWindow_H__

#include "OgreRenderWindow.h"

namespace Ogre {

    class BridgeWindow : public RenderWindow
    {
	protected:

		bool mClosed;
		void reposition(int left, int top);
		void resize(unsigned int width, unsigned int height);
		void windowMovedOrResized();
		void switchFullScreen(bool fullscreen);


	public:
        BridgeWindow();
        virtual  ~BridgeWindow();
		bool requiresTextureFlipping() const;
		void destroy(void);
		bool isClosed(void) const;

	    //Moved this from EGLWindow because it has some native calls.
        void create(const String& name, unsigned int width, unsigned int height,
			bool fullScreen, const NameValuePairList *miscParams);

		void copyContentsToMemory(const PixelBox &dst, FrameBuffer buffer);
	};
}

#endif
