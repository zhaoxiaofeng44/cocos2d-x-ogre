
/* Copyright 2013 Jonne Nauha / jonne@adminotech.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "OgreBridgeRenderSystem.h"
#include "components/OgreBridgeBufferManager.h"
#include "components/OgreBridgeTextureManager.h"
#include "components/OgreBridgeGpuProgramManager.h"

namespace Ogre
{
    BridgeRenderSystem::BridgeRenderSystem() :
        RenderSystem(),
        mTextureManager(0),
        mHardwareBufferManager(0)
    {
        LogManager::getSingleton().logMessage("BridgeRenderSystem ctor", Ogre::LML_TRIVIAL);

        reinitialise();
    }
    
    BridgeRenderSystem::~BridgeRenderSystem()
    {
        LogManager::getSingleton().logMessage("BridgeRenderSystem dtor", Ogre::LML_TRIVIAL);

        shutdown();
    }
   
    const String& BridgeRenderSystem::getName(void) const
    {
        static String renderSystemName("Bridge Rendering Subsystem");
        return renderSystemName;
    }

    RenderWindow* BridgeRenderSystem::_initialise(bool autoCreateWindow, const String& windowTitle)
    {
        OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
            String("BridgeRenderSystem cannot be initialized. This indicates Ogre::Root::initialize() was called and you are about to create a rendering window. ") +
            String("This rendering plugin is for headless operation only."),
            "BridgeRenderSystem::_initialise");
        return 0;
    }

    void BridgeRenderSystem::reinitialise()
    {
        LogManager::getSingleton().logMessage("BridgeRenderSystem::reinitialise", Ogre::LML_TRIVIAL);

        shutdown();

        mTextureManager = OGRE_NEW BridgeHardwareTextureManager();
        mHardwareBufferManager = OGRE_NEW BridgeHardwareBufferManager();
		mRealCapabilities = createRenderSystemCapabilities();
		if (!mUseCustomCapabilities)
                mCurrentCapabilities = mRealCapabilities;
		 mGpuProgramManager = OGRE_NEW BridgeGpuProgramManager();
    }
	  
	void BridgeRenderSystem::shutdown(void)
    {
        if (!mTextureManager && !mHardwareBufferManager)
            return;

        LogManager::getSingleton().logMessage("BridgeRenderSystem::shutdown", Ogre::LML_TRIVIAL);

        if (mTextureManager)
            OGRE_DELETE mTextureManager;
        mTextureManager = 0;
        if (mHardwareBufferManager)
            OGRE_DELETE mHardwareBufferManager;
        mHardwareBufferManager = 0;

		if (mGpuProgramManager)
			OGRE_DELETE mGpuProgramManager;
		mGpuProgramManager = 0;
    }

	RenderSystemCapabilities* BridgeRenderSystem::createRenderSystemCapabilities() const
    {
        RenderSystemCapabilities* rsc = OGRE_NEW RenderSystemCapabilities();

        rsc->setCategoryRelevant(CAPS_CATEGORY_GL, true);
        rsc->setDriverVersion(mDriverVersion);

        /*const char* deviceName = (const char*)glGetString(GL_RENDERER);
		const char* vendorName = (const char*)glGetString(GL_VENDOR);        
        if (deviceName)
        {
            rsc->setDeviceName(deviceName);
        }*/

        rsc->setRenderSystemName(getName());

		// Determine vendor
		/*if (strstr(vendorName, "Imagination Technologies"))
			rsc->setVendor(GPU_IMAGINATION_TECHNOLOGIES);
		else if (strstr(vendorName, "Apple Computer, Inc."))
			rsc->setVendor(GPU_APPLE);  // iOS Simulator
		else if (strstr(vendorName, "NVIDIA"))
			rsc->setVendor(GPU_NVIDIA);
        else
            rsc->setVendor(GPU_UNKNOWN);*/

        // Multitexturing support and set number of texture units
        GLint units;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &units);
        rsc->setNumTextureUnits(units);

        // Check for hardware stencil support and set bit depth
        GLint stencil;

        glGetIntegerv(GL_STENCIL_BITS, &stencil);
        //CHECK_GL_ERROR_DEBUG();

        if(stencil)
        {
            rsc->setCapability(RSC_HWSTENCIL);
			rsc->setCapability(RSC_TWO_SIDED_STENCIL);
            rsc->setStencilBufferBitDepth(stencil);
        }

        // Scissor test is standard
        rsc->setCapability(RSC_SCISSOR_TEST);

        // Vertex Buffer Objects are always supported by OpenGL ES
        rsc->setCapability(RSC_VBO);

        rsc->setCapability(RSC_FBO);
        rsc->setCapability(RSC_HWRENDER_TO_TEXTURE);
        rsc->setNumMultiRenderTargets(1);

        // Cube map
        rsc->setCapability(RSC_CUBEMAPPING);

        // Stencil wrapping
        rsc->setCapability(RSC_STENCIL_WRAP);

        // GL always shares vertex and fragment texture units (for now?)
        rsc->setVertexTextureUnitsShared(true);

        // Hardware support mipmapping
        rsc->setCapability(RSC_AUTOMIPMAP);

        // Blending support
        rsc->setCapability(RSC_BLENDING);
        rsc->setCapability(RSC_ADVANCED_BLEND_OPERATIONS);

        // DOT3 support is standard
        rsc->setCapability(RSC_DOT3);
        
        // Point size
        GLfloat psRange[2] = {0.0, 0.0};
        glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, psRange);
        CHECK_GL_ERROR_DEBUG();
        rsc->setMaxPointSize(psRange[1]);

        // Point sprites
        rsc->setCapability(RSC_POINT_SPRITES);
        rsc->setCapability(RSC_POINT_EXTENDED_PARAMETERS);
		
        // GLSL ES is always supported in GL ES 2
        rsc->addShaderProfile("glsles");
        LogManager::getSingleton().logMessage("GLSL ES support detected");

#if !OGRE_NO_GLES2_CG_SUPPORT
        rsc->addShaderProfile("cg");
        rsc->addShaderProfile("ps_2_0");
        rsc->addShaderProfile("vs_2_0");
#endif

        // UBYTE4 always supported
        rsc->setCapability(RSC_VERTEX_FORMAT_UBYTE4);

        // Infinite far plane always supported
        rsc->setCapability(RSC_INFINITE_FAR_PLANE);

        // Vertex/Fragment Programs
        rsc->setCapability(RSC_VERTEX_PROGRAM);
        rsc->setCapability(RSC_FRAGMENT_PROGRAM);

#if OGRE_NO_GLES2_GLSL_SUPPORT == 0

        GLfloat floatConstantCount = 0;
        glGetFloatv(GL_MAX_VERTEX_UNIFORM_VECTORS, &floatConstantCount);
        rsc->setVertexProgramConstantFloatCount((Ogre::ushort)floatConstantCount);
        rsc->setVertexProgramConstantBoolCount((Ogre::ushort)floatConstantCount);
        rsc->setVertexProgramConstantIntCount((Ogre::ushort)floatConstantCount);

        // Fragment Program Properties
        floatConstantCount = 0;
        glGetFloatv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &floatConstantCount);
        rsc->setFragmentProgramConstantFloatCount((Ogre::ushort)floatConstantCount);
        rsc->setFragmentProgramConstantBoolCount((Ogre::ushort)floatConstantCount);
        rsc->setFragmentProgramConstantIntCount((Ogre::ushort)floatConstantCount);
#endif
        // Geometry programs are not supported, report 0
        rsc->setGeometryProgramConstantFloatCount(0);
        rsc->setGeometryProgramConstantBoolCount(0);
        rsc->setGeometryProgramConstantIntCount(0);
        
        // Check for Float textures
#if GL_OES_texture_float || GL_OES_texture_half_float
        rsc->setCapability(RSC_TEXTURE_FLOAT);
#endif

        // Alpha to coverage always 'supported' when MSAA is available
        // although card may ignore it if it doesn't specifically support A2C
        rsc->setCapability(RSC_ALPHA_TO_COVERAGE);
		
		// No point sprites, so no size
		rsc->setMaxPointSize(0.f);
        
        return rsc;
    }

    ConfigOptionMap& BridgeRenderSystem::getConfigOptions(void)
    {
        return mHeadlessOptions;
    }

    RenderWindow* BridgeRenderSystem::_createRenderWindow(const String &name, unsigned int width, unsigned int height, bool fullScreen, const NameValuePairList *miscParams)
    {
        OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
            "BridgeRenderSystem cannot create rendering windows. This rendering plugin is for headless operation only.",
            "BridgeRenderSystem::_createRenderWindow");
        return 0;
    }

    bool BridgeRenderSystem::_createRenderWindows(const RenderWindowDescriptionList& renderWindowDescriptions, RenderWindowList& createdWindows)
    {
        OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
            "BridgeRenderSystem cannot create rendering windows. This rendering plugin is for headless operation only.",
            "BridgeRenderSystem::_createRenderWindows");
        return false;
    }
}
