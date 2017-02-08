#ifndef OGRE_BRIDGE_BUFFER_MANAGER_H
#define OGRE_BRIDGE_BUFFER_MANAGER_H


#include "cocos2d.h"

#include "OgrePrerequisites.h"
#include "OgreHardwareBufferManager.h"
#include "OgreHardwareVertexBuffer.h"
#include "OgreHardwareIndexBuffer.h"

NS_CC_BEGIN

	class EventListenerCustom;
NS_CC_END

namespace Ogre {
	
    class BridgeHardwareVertexBuffer : public HardwareVertexBuffer
    {
        private:

			mutable GLuint _vbo;
			cocos2d::EventListenerCustom * _recreateVBOEventListener;

            // Scratch buffer handling
            bool mLockedToScratch;
            size_t mScratchOffset;
            size_t mScratchSize;
            void* mScratchPtr;
            bool mScratchUploadOnUnlock;
			
        protected:
       
            void* lockImpl(size_t offset, size_t length, LockOptions options);

            void unlockImpl(void);

        public:
            BridgeHardwareVertexBuffer(HardwareBufferManagerBase* mgr, size_t vertexSize, size_t numVertices,
                                   HardwareBuffer::Usage usage, bool useShadowBuffer);
            virtual ~BridgeHardwareVertexBuffer();

            void readData(size_t offset, size_t length, void* pDest);

            void writeData(size_t offset, size_t length, 
                           const void* pSource, bool discardWholeBuffer = false);

            void _updateFromShadow(void);

            inline GLuint getGLBufferId(void) const { return _vbo; }
    };


	class BridgeHardwareIndexBuffer : public HardwareIndexBuffer
    {
        private:

			mutable GLuint _vbo;
			cocos2d::EventListenerCustom * _recreateVBOEventListener;

            bool mLockedToScratch;
            size_t mScratchOffset;
            size_t mScratchSize;
            void* mScratchPtr;
            bool mScratchUploadOnUnlock;
			
        protected:

            void* lockImpl(size_t offset, size_t length, LockOptions options);

            void unlockImpl(void);

        public:
            BridgeHardwareIndexBuffer(HardwareBufferManagerBase* mgr, IndexType idxType, size_t numIndexes,
                                  HardwareBuffer::Usage usage,
                                  bool useShadowBuffer);
            virtual ~BridgeHardwareIndexBuffer();

            void readData(size_t offset, size_t length, void* pDest);

            void writeData(size_t offset, size_t length, 
                const void* pSource, bool discardWholeBuffer = false);

            void _updateFromShadow(void);

			inline GLuint getGLBufferId(void) const { return _vbo; }
    };


	 // Default threshold at which glMapBuffer becomes more efficient than glBufferSubData (32k?)
    #   define OGRE_GL_DEFAULT_MAP_BUFFER_THRESHOLD (1024 * 32)

    /** Implementation of HardwareBufferManager for OpenGL ES. */
    class _OgreExport BridgeHardwareBufferManagerBase : public HardwareBufferManagerBase
    {
        protected:
            char* mScratchBufferPool;
            OGRE_MUTEX(mScratchMutex)
            size_t mMapBufferThreshold;

        public:
            BridgeHardwareBufferManagerBase();
            virtual ~BridgeHardwareBufferManagerBase();
            /// Creates a vertex buffer
            HardwareVertexBufferSharedPtr createVertexBuffer(size_t vertexSize,
                size_t numVerts, HardwareBuffer::Usage usage, bool useShadowBuffer = false);
            /// Create a hardware vertex buffer
            HardwareIndexBufferSharedPtr createIndexBuffer(
                HardwareIndexBuffer::IndexType itype, size_t numIndexes,
                HardwareBuffer::Usage usage, bool useShadowBuffer = false);
	        /// Create a render to vertex buffer
    	    RenderToVertexBufferSharedPtr createRenderToVertexBuffer();

            /** Allocator method to allow us to use a pool of memory as a scratch
                area for hardware buffers. This is because glMapBuffer is incredibly
                inefficient, seemingly no matter what options we give it. So for the
                period of lock/unlock, we will instead allocate a section of a local
                memory pool, and use glBufferSubDataARB / glGetBufferSubDataARB
                instead.
            */
            void* allocateScratch(uint32 size);

            /// @see allocateScratch
            void deallocateScratch(void* ptr);

    		/** Threshold after which glMapBuffer is used and not glBufferSubData
            */
            size_t getGLMapBufferThreshold() const;
            void setGLMapBufferThreshold( const size_t value );
    };

	/// GLES2HardwareBufferManagerBase as a Singleton
	class _OgreExport BridgeHardwareBufferManager : public HardwareBufferManager
	{
	public:
		BridgeHardwareBufferManager()
			: HardwareBufferManager(OGRE_NEW BridgeHardwareBufferManagerBase()) 
		{

		}
		~BridgeHardwareBufferManager()
		{
			OGRE_DELETE mImpl;
		}

		/** Allocator method to allow us to use a pool of memory as a scratch
		area for hardware buffers. This is because glMapBuffer is incredibly
		inefficient, seemingly no matter what options we give it. So for the
		period of lock/unlock, we will instead allocate a section of a local
		memory pool, and use glBufferSubDataARB / glGetBufferSubDataARB
		instead.
		*/
		void* allocateScratch(uint32 size)
		{
			return static_cast<BridgeHardwareBufferManagerBase*>(mImpl)->allocateScratch(size);
		}

		/// @see allocateScratch
		void deallocateScratch(void* ptr)
		{
			static_cast<BridgeHardwareBufferManagerBase*>(mImpl)->deallocateScratch(ptr);
		}

        /** Threshold after which glMapBuffer is used and not glBufferSubData
		*/
		size_t getGLMapBufferThreshold() const
		{
			return static_cast<BridgeHardwareBufferManagerBase*>(mImpl)->getGLMapBufferThreshold();
		}
		void setGLMapBufferThreshold( const size_t value )
		{
			static_cast<BridgeHardwareBufferManagerBase*>(mImpl)->setGLMapBufferThreshold(value);
		}

	};
}
#endif//OGREAPP_H
