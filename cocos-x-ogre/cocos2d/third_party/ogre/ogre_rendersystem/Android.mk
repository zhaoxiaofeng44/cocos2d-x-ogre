LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := rendersystem_static

LOCAL_MODULE_FILENAME := librendersystem

# ogre_rendersystem	
LOCAL_OGRE_RENDER_GLES_INCLUDE := \
	$(LOCAL_PATH)/include	\
	$(LOCAL_PATH)/include/common
	
LOCAL_OGRE_RENDER_GLES_SRC_FILES := \
	src/common/OgreCommonGLContext.cpp  \
    src/common/OgreCommonGLSupport.cpp  \
    src/common/OgreCommonWindow.cpp  \
    src/OgreGLES2Context.cpp  \
    src/OgreGLES2DefaultHardwareBufferManager.cpp  \
    src/OgreGLES2DepthBuffer.cpp  \
    src/OgreGLES2EngineDll.cpp  \
    src/OgreGLES2FBOMultiRenderTarget.cpp  \
    src/OgreGLES2FBORenderTexture.cpp  \
    src/OgreGLES2FrameBufferObject.cpp  \
    src/OgreGLES2GpuProgram.cpp  \
    src/OgreGLES2GpuProgramManager.cpp  \
    src/OgreGLES2HardwareBufferManager.cpp  \
    src/OgreGLES2HardwareIndexBuffer.cpp  \
    src/OgreGLES2HardwareOcclusionQuery.cpp  \
    src/OgreGLES2HardwarePixelBuffer.cpp  \
    src/OgreGLES2HardwareVertexBuffer.cpp  \
    src/OgreGLES2PixelFormat.cpp  \
    src/OgreGLES2Plugin.cpp  \
    src/OgreGLES2RenderSystem.cpp  \
    src/OgreGLES2RenderTexture.cpp  \
    src/OgreGLES2Support.cpp  \
    src/OgreGLES2Texture.cpp  \
    src/OgreGLES2TextureManager.cpp

LOCAL_EXPORT_C_INCLUDES :=
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/include 
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../include 
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/include/common

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_C_INCLUDES += $(LOCAL_OGRE_RENDER_GLES_INCLUDE)

LOCAL_SRC_FILES := 
LOCAL_SRC_FILES += $(LOCAL_OGRE_RENDER_GLES_SRC_FILES)    
					
					
LOCAL_WHOLE_STATIC_LIBRARIES := ogremain_static 

include $(BUILD_STATIC_LIBRARY)
$(call import-module,../third_party/ogre/ogre_main)