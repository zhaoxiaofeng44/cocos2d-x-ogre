#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# Configure BSP SceneManager build

set (HEADER_FILES
  include/OgreAreaEmitter.h
  include/OgreBoxEmitterFactory.h
  include/OgreBoxEmitter.h
  include/OgreColourFaderAffector2.h
  include/OgreColourFaderAffectorFactory2.h
  include/OgreColourFaderAffectorFactory.h
  include/OgreColourFaderAffector.h
  include/OgreColourImageAffectorFactory.h
  include/OgreColourImageAffector.h
  include/OgreColourInterpolatorAffectorFactory.h
  include/OgreColourInterpolatorAffector.h
  include/OgreCylinderEmitterFactory.h
  include/OgreCylinderEmitter.h
  include/OgreDeflectorPlaneAffectorFactory.h
  include/OgreDeflectorPlaneAffector.h
  include/OgreDirectionRandomiserAffectorFactory.h
  include/OgreDirectionRandomiserAffector.h
  include/OgreEllipsoidEmitterFactory.h
  include/OgreEllipsoidEmitter.h
  include/OgreHollowEllipsoidEmitterFactory.h
  include/OgreHollowEllipsoidEmitter.h
  include/OgreLinearForceAffectorFactory.h
  include/OgreLinearForceAffector.h
  include/OgreParticleFXPlugin.h
  include/OgreParticleFXPrerequisites.h
  include/OgrePointEmitterFactory.h
  include/OgrePointEmitter.h
  include/OgreRingEmitterFactory.h
  include/OgreRingEmitter.h
  include/OgreRotationAffectorFactory.h
  include/OgreRotationAffector.h
  include/OgreScaleAffectorFactory.h
  include/OgreScaleAffector.h
)

set (SOURCE_FILES
  src/OgreAreaEmitter.cpp
  src/OgreBoxEmitter.cpp
  src/OgreColourFaderAffector2.cpp
  src/OgreColourFaderAffector.cpp
  src/OgreColourImageAffector.cpp
  src/OgreColourInterpolatorAffector.cpp
  src/OgreCylinderEmitter.cpp
  src/OgreDeflectorPlaneAffector.cpp
  src/OgreDirectionRandomiserAffector.cpp
  src/OgreEllipsoidEmitter.cpp
  src/OgreHollowEllipsoidEmitter.cpp
  src/OgreLinearForceAffector.cpp
  src/OgreParticleFX.cpp
  src/OgreParticleFXPlugin.cpp
  src/OgrePointEmitter.cpp
  src/OgreRingEmitter.cpp
  src/OgreRotationAffector.cpp
  src/OgreScaleAffector.cpp
)

include_directories("include")
include_directories("../ogre_main/include")

# Add platform specific files
if (OGRE_BUILD_PLATFORM_NACL)

  set(PLATFORM_LIBS nosys)
  set(PLATFORM_HEADER_INSTALL "NaCl")
elseif (WIN32)

  include_directories("../include/win32")

  set(PLATFORM_LIBS "")
  set(PLATFORM_HEADER_INSTALL "WIN32")

elseif (APPLE)
  if (OGRE_BUILD_PLATFORM_APPLE_IOS)
    include_directories("include/iOS")
	
	include_directories("../include/mac")

    set(PLATFORM_LIBS "")
    set(PLATFORM_HEADER_INSTALL "iOS")
  else ()

	include_directories("../include/ios")

    set(PLATFORM_LIBS "")
    set(PLATFORM_HEADER_INSTALL "OSX")
  endif ()
elseif (UNIX)
  
  set(PLATFORM_LIBS "")
  set(PLATFORM_HEADER_INSTALL "GLX")
endif()

add_library(ogre_particlefx STATIC
  ${SOURCE_FILES}
)

set_target_properties(ogre_particlefx
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

