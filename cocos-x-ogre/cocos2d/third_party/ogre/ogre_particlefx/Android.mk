LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := particlefx_static

LOCAL_MODULE_FILENAME := libogreparticlefx


LOCAL_LDLIBS := -lGLESv2 \
                -llog \
                -lz \
                -landroid

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -llog \
                       -lz \
                       -landroid


LOCAL_CFLAGS   :=  -fexceptions -DUSE_FILE32API
LOCAL_CPPFLAGS := -fexceptions -Wno-deprecated-declarations
LOCAL_EXPORT_CFLAGS   := -lstdc++ -fexceptions
LOCAL_EXPORT_CPPFLAGS := -lstdc++ -fexceptions


# ogre_particlefx
LOCAL_OGRE_PARTICLE_FX_INCLUDE := \
	$(LOCAL_PATH)/include
	
LOCAL_OGRE_PARTICLE_FX_SRC_FILES := \
    src/OgreAreaEmitter.cpp  \
    src/OgreBoxEmitter.cpp  \
    src/OgreColourFaderAffector2.cpp  \
    src/OgreColourFaderAffector.cpp  \
    src/OgreColourImageAffector.cpp  \
    src/OgreColourInterpolatorAffector.cpp  \
    src/OgreCylinderEmitter.cpp  \
    src/OgreDeflectorPlaneAffector.cpp  \
    src/OgreDirectionRandomiserAffector.cpp  \
    src/OgreEllipsoidEmitter.cpp  \
    src/OgreHollowEllipsoidEmitter.cpp  \
    src/OgreLinearForceAffector.cpp  \
    src/OgreParticleFX.cpp  \
    src/OgreParticleFXPlugin.cpp  \
    src/OgrePointEmitter.cpp  \
    src/OgreRingEmitter.cpp  \
    src/OgreRotationAffector.cpp  \
    src/OgreScaleAffector.cpp
	

LOCAL_EXPORT_C_INCLUDES := \
	$(LOCAL_PATH)/../../../cocos \
	$(LOCAL_PATH)/../../../cocos/platform/android \
	$(LOCAL_PATH)/../../../external/freetype2/include/android \

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/include 
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../include 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_C_INCLUDES += $(LOCAL_OGRE_PARTICLE_FX_INCLUDE)

LOCAL_SRC_FILES := \
LOCAL_SRC_FILES += $(LOCAL_OGRE_PARTICLE_FX_SRC_FILES)    
					
					
LOCAL_WHOLE_STATIC_LIBRARIES := ogremain_static 

include $(BUILD_STATIC_LIBRARY)
$(call import-module,../third_party/ogre/ogre_main)