LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := encapsulation_static

LOCAL_MODULE_FILENAME := libencapsulation

# ogre_rendersystem	
LOCAL_OGRE_COCOS_INCLUDE := \
	$(LOCAL_PATH)/ogre
	
LOCAL_OGRE_COCOS_SRC_FILES := \
	ogre/OgreOgreSprite3D.cpp \
    ogre/OgreParticleSprite3D.cpp  \
    ogre/OgreOgreManager.cpp  \
    ogre/OgreNodeProtocol.cpp  \
    ogre/components/OgreBridgeSceneManager.cpp  \
	ogre/components/OgreBridgeFileSystemArchive.cpp  \
    ogre/components/OgreBridgeGpuProgram.cpp  \
	ogre/components/OgreBridgeGpuProgramManager.cpp \
    ogre/components/OgreBridgeImageCodec.cpp \
	ogre/components/OgreBridgeRenderSystem.cpp \
	ogre/components/OgreBridgeBufferManager.cpp \
	ogre/components/OgreBridgeTextureManager.cpp \
	ogre/components/OgreBridgeRenderSystemPlugin.cpp \
	ogre/components/OgreBridgeSceneManager.cpp \
	ogre/components/OgreBridgeWindow.cpp \
	extension/GroupNode3D.cpp \
	extension/CameraBillBoard.cpp \
	extension/BonesAnimate3D.cpp \
	extension/ScheduleAction.cpp \
	extension/ScriptCallFuncHelper.cpp \
	
	
LOCAL_EXPORT_C_INCLUDES :=
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/ogre
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/extension

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(LOCAL_OGRE_COCOS_INCLUDE)

LOCAL_SRC_FILES := 
LOCAL_SRC_FILES += $(LOCAL_OGRE_COCOS_SRC_FILES)    
	
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static 	
LOCAL_WHOLE_STATIC_LIBRARIES += ogremain_static 
LOCAL_WHOLE_STATIC_LIBRARIES += particlefx_static 
LOCAL_WHOLE_STATIC_LIBRARIES += universe_static 

include $(BUILD_STATIC_LIBRARY)
$(call import-module,.)
$(call import-module,../third_party/ogre/ogre_main)
$(call import-module,../third_party/ogre/ogre_particlefx)
$(call import-module,../third_party/ogre/ogre_pu)