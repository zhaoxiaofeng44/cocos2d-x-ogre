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
 	OgreRenderScene.h 
	OgreOgreSprite3D.h
    OgreParticleSprite3D.h
    OgreOgreManager.h
    OgreNodeProtocol.h
    OgreBridgeSceneManager.h
    OgreBridgeImageCodec.h
    OgreBridgeFileSystemArchive.h
)

set(SOURCE_FILES
  	OgreRenderScene.cpp 
	OgreOgreSprite3D.cpp
    OgreParticleSprite3D.cpp
    OgreOgreManager.cpp
    OgreNodeProtocol.cpp
    OgreBridgeSceneManager.cpp
    OgreBridgeImageCodec.cpp
    OgreBridgeFileSystemArchive.cpp
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(./ogre/ogre_main)

add_subdirectory(./ogre/ogre_particlefx)

add_subdirectory(./ogre/ogre_pu)

add_subdirectory(./ogre/ogre_rendersystem)

add_library(thirdparty STATIC
  ${SOURCE_FILES}
)

set_target_properties(thirdparty
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
