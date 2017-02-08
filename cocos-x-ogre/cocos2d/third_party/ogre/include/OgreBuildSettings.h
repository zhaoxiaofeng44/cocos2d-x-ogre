#ifndef __Custom_Config_H_
#define __Custom_Config_H_

// CMake auto-generated configuration options


#define OGRE_STATIC_LIB

#define OGRE_USE_COCOS_GLES  //use gles 2.0 enum

#define OGRE_NO_READ_WRITE 1  //not reay or write file in ogre

#define OGRE_DOUBLE_PRECISION 0  //use as float

#define OGRE_MEMORY_ALLOCATOR 1

#define OGRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR 0 //use custom alloc -- android or ios is failed

#define OGRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR 0      //use custom string -- not as std::string

#define OGRE_MEMORY_TRACKER_DEBUG_MODE 0

#define OGRE_MEMORY_TRACKER_RELEASE_MODE 0

#define OGRE_THREAD_SUPPORT 0

#define OGRE_THREAD_PROVIDER 0

#define OGRE_NO_FREEIMAGE 1  //no use the extends lib  --> freeimage

#define OGRE_NO_DDS_CODEC 1

#define OGRE_NO_PVRTC_CODEC 1

#define OGRE_NO_ZIP_ARCHIVE 1 //no use the extends lib  --> zip

#define OGRE_NO_VIEWPORT_ORIENTATIONMODE 1

#define OGRE_NO_GLES2_CG_SUPPORT 1

#define OGRE_NO_GLES2_GLSL_SUPPORT 1

#define OGRE_NO_GLES2_GLSL_OPTIMISER 1

#define OGRE_USE_NEW_COMPILERS 1

#define OGRE_USE_BOOST 0

#define OGRE_PROFILING 0

#define RTSHADER_SYSTEM_BUILD_CORE_SHADERS

#define RTSHADER_SYSTEM_BUILD_EXT_SHADERS


#endif
