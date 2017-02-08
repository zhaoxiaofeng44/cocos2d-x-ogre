#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# Configure OpenGL ES 2.0 RenderSystem build

set(HEADER_FILES
  include/OgreGLES2Context.h
  include/OgreGLES2DefaultHardwareBufferManager.h
  include/OgreGLES2DepthBuffer.h
  include/OgreGLES2FBOMultiRenderTarget.h
  include/OgreGLES2FBORenderTexture.h
  include/OgreGLES2FrameBufferObject.h
  include/OgreGLES2GpuProgram.h
  include/OgreGLES2GpuProgramManager.h
  include/OgreGLES2HardwareBufferManager.h
  include/OgreGLES2HardwareIndexBuffer.h
  include/OgreGLES2HardwareOcclusionQuery.h
  include/OgreGLES2HardwarePixelBuffer.h
  include/OgreGLES2HardwareVertexBuffer.h
  include/OgreGLES2PixelFormat.h
  include/OgreGLES2Plugin.h
  include/OgreGLES2Prerequisites.h
  include/OgreGLES2RenderSystem.h
  include/OgreGLES2RenderTexture.h
  include/OgreGLES2Support.h
  include/OgreGLES2Texture.h
  include/OgreGLES2TextureManager.h
)

set(SOURCE_FILES
  src/OgreGLES2Context.cpp
  src/OgreGLES2DefaultHardwareBufferManager.cpp
  src/OgreGLES2DepthBuffer.cpp
  src/OgreGLES2EngineDll.cpp
  src/OgreGLES2FBOMultiRenderTarget.cpp
  src/OgreGLES2FBORenderTexture.cpp
  src/OgreGLES2FrameBufferObject.cpp
  src/OgreGLES2GpuProgram.cpp
  src/OgreGLES2GpuProgramManager.cpp
  src/OgreGLES2HardwareBufferManager.cpp
  src/OgreGLES2HardwareIndexBuffer.cpp
  src/OgreGLES2HardwareOcclusionQuery.cpp
  src/OgreGLES2HardwarePixelBuffer.cpp
  src/OgreGLES2HardwareVertexBuffer.cpp
  src/OgreGLES2PixelFormat.cpp
  src/OgreGLES2Plugin.cpp
  src/OgreGLES2RenderSystem.cpp
  src/OgreGLES2RenderTexture.cpp
  src/OgreGLES2Support.cpp
  src/OgreGLES2Texture.cpp
  src/OgreGLES2TextureManager.cpp
)

set(GLSLES_FILES
  src/GLSLES/include/OgreGLSLESExtSupport.h
  src/GLSLES/include/OgreGLSLESGpuProgram.h
  src/GLSLES/include/OgreGLSLESLinkProgram.h
  src/GLSLES/include/OgreGLSLESLinkProgramManager.h
  src/GLSLES/include/OgreGLSLESPreprocessor.h
  src/GLSLES/include/OgreGLSLESProgram.h
  src/GLSLES/include/OgreGLSLESProgramCommon.h
  src/GLSLES/include/OgreGLSLESProgramFactory.h
  src/GLSLES/include/OgreGLSLESProgramManagerCommon.h
  src/GLSLES/include/OgreGLSLESProgramPipeline.h
  src/GLSLES/include/OgreGLSLESProgramPipelineManager.h
  src/GLSLES/src/OgreGLSLESExtSupport.cpp
  src/GLSLES/src/OgreGLSLESGpuProgram.cpp
  src/GLSLES/src/OgreGLSLESLinkProgram.cpp
  src/GLSLES/src/OgreGLSLESLinkProgramManager.cpp
  src/GLSLES/src/OgreGLSLESPreprocessor.cpp
  src/GLSLES/src/OgreGLSLESProgram.cpp
  src/GLSLES/src/OgreGLSLESProgramCommon.cpp
  src/GLSLES/src/OgreGLSLESProgramFactory.cpp
  src/GLSLES/src/OgreGLSLESProgramManagerCommon.cpp
  src/GLSLES/src/OgreGLSLESProgramPipeline.cpp
  src/GLSLES/src/OgreGLSLESProgramPipelineManager.cpp
)

if (OGRE_CONFIG_ENABLE_GLES2_CG_SUPPORT)
  set(GLSLES_FILES 
    src/GLSLES/src/OgreGLSLESCgProgram.cpp
    src/GLSLES/src/OgreGLSLESCgProgramFactory.cpp
    ${GLSLES_FILES}
  )
endif (OGRE_CONFIG_ENABLE_GLES2_CG_SUPPORT)


set(PLATFORM_HEADERS 
	include/common/OgreCommonWindow.h
	include/common/OgreCommonGLSupport.h
	include/common/OgreCommonGLContext.h
)

set(PLATFORM_SOURCES
	src/common/OgreCommonWindow.cpp
	src/common/OgreCommonGLSupport.cpp
	src/common/OgreCommonGLContext.cpp
)

include_directories("../include")
include_directories("../ogre_main/include")

include_directories("include")
include_directories("include/common")

include_directories("src/GLSLES/include")

include_directories("../../../cocos")

# Add platform specific files
if (OGRE_BUILD_PLATFORM_NACL)

  set(PLATFORM_LIBS nosys)
  set(PLATFORM_HEADER_INSTALL "NaCl")
elseif (WIN32)

  include_directories("../../../cocos/platform/win32")
  include_directories("../../../external/freetype2/include/win32")
  include_directories("../../../external/win32-specific/gles/include/OGLES")

  set(PLATFORM_HEADER_INSTALL "WIN32")

elseif (APPLE)
  if (OGRE_BUILD_PLATFORM_APPLE_IOS)
    include_directories("include/iOS")
	
	include_directories("../../../cocos/platform/ios")
    include_directories("../../../external/freetype2/include/ios")

    set(PLATFORM_LIBS "")
    set(PLATFORM_HEADER_INSTALL "iOS")
  else ()
    include_directories("include/OSX")
	
	include_directories("../../../cocos/platform/mac")
    include_directories("../../../external/freetype2/include/mac")
	
    set(PLATFORM_LIBS "")
    set(PLATFORM_HEADER_INSTALL "OSX")
  endif ()
elseif (UNIX)
  
  include_directories("../../../cocos/platform/linux")
  include_directories("../../../external/freetype2/include/linux")
  
  set(PLATFORM_LIBS "")
  set(PLATFORM_HEADER_INSTALL "GLX")
endif()

add_library(ogre_rendersystem STATIC
  ${SOURCE_FILES}
  ${GLSLES_FILES}
  ${PLATFORM_SOURCES}
)

set_target_properties(ogre_rendersystem
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

