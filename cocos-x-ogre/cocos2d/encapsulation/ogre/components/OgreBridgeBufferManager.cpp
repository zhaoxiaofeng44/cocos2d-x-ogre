
#include "OgreStableHeaders.h"
#include "OgreBridgeBufferManager.h"

#include "renderer/CCVertexIndexBuffer.h"

#include "base/CCEventType.h"
#include "base/CCEventListenerCustom.h"
#include "base/CCEventDispatcher.h"
#include "base/CCDirector.h"

namespace Ogre {

    inline GLenum getGLUsageByHardwareUsage(unsigned int usage)
    {
        switch(usage)
        {
            case HardwareBuffer::HBU_STATIC:
            case HardwareBuffer::HBU_STATIC_WRITE_ONLY:
                return GL_STATIC_DRAW;
            case HardwareBuffer::HBU_DYNAMIC:
            case HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY:
                return GL_DYNAMIC_DRAW;
            case HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE:
                return GL_STREAM_DRAW;
            default:
                return GL_DYNAMIC_DRAW;
        };
    }

    inline GLenum getGLTypeByHardwareType(unsigned int type)
    {
        switch(type)
        {
            case VET_FLOAT1:
            case VET_FLOAT2:
            case VET_FLOAT3:
            case VET_FLOAT4:
                return GL_FLOAT;
            case VET_SHORT1:
            case VET_SHORT2:
            case VET_SHORT3:
            case VET_SHORT4:
                return GL_SHORT;
            case VET_COLOUR:
            case VET_COLOUR_ABGR:
            case VET_COLOUR_ARGB:
            case VET_UBYTE4:
                return GL_UNSIGNED_BYTE;
            default:
                return 0;
        };
    }

	BridgeHardwareVertexBuffer::BridgeHardwareVertexBuffer(HardwareBufferManagerBase* mgr, 
													   size_t vertexSize,
                                                       size_t numVertices,
                                                       HardwareBuffer::Usage usage,
                                                       bool useShadowBuffer)
        : HardwareVertexBuffer(mgr, vertexSize, numVertices, usage, false, useShadowBuffer)
    {

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		CHECK_GL_ERROR_DEBUG();
		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, nullptr, GL_STATIC_DRAW);
		CHECK_GL_ERROR_DEBUG();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		   
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		auto callBack = [this](cocos2d::EventCustom* event)
		{
			this->mShadowUpdated = true;
			this->_updateFromShadow();
		};
		_recreateVBOEventListener = cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_RENDERER_RECREATED, callBack);
#endif

    }

    BridgeHardwareVertexBuffer::~BridgeHardwareVertexBuffer()
    {
    if(glIsBuffer(_vbo))
		{
			glDeleteBuffers(1, &_vbo);
			_vbo = 0;
		}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(_recreateVBOEventListener);
#endif
    }

    void* BridgeHardwareVertexBuffer::lockImpl(size_t offset,
                                           size_t length,
                                           LockOptions options)
    {
        if (mIsLocked)
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                        "Invalid attempt to lock an index buffer that has already been locked",
                        "BridgeHardwareVertexBuffer::lock");
        }

        void* retPtr = 0;

		BridgeHardwareBufferManager* glBufManager = static_cast<BridgeHardwareBufferManager*>(HardwareBufferManager::getSingletonPtr());

		// Try to use scratch buffers for smaller buffers
        if (length < glBufManager->getGLMapBufferThreshold())
        {
			// if this fails, we fall back on mapping
            retPtr = glBufManager->allocateScratch((uint32)length);

            if (retPtr)
            {
                mLockedToScratch = true;
                mScratchOffset = offset;
                mScratchSize = length;
                mScratchPtr = retPtr;
                mScratchUploadOnUnlock = (options != HBL_READ_ONLY);

                if (options != HBL_DISCARD)
                {
					// have to read back the data before returning the pointer
                    readData(offset, length, retPtr);
                }
            }
        }
        else
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                        "Invalid Buffer lockSize",
                        "BridgeHardwareVertexBuffer::lock");
        }

		mIsLocked = true;
        return retPtr;
    }

    void BridgeHardwareVertexBuffer::unlockImpl(void)
    {
        if (mLockedToScratch)
        {
            if (mScratchUploadOnUnlock)
            {
                // have to write the data back to vertex buffer
                writeData(mScratchOffset, mScratchSize, mScratchPtr,
                          mScratchOffset == 0 && mScratchSize == getSizeInBytes());
            }

            static_cast<BridgeHardwareBufferManager*>(
                HardwareBufferManager::getSingletonPtr())->deallocateScratch(mScratchPtr);

            mLockedToScratch = false;
        }
        else
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                        "Only locking to scratch is supported",
                        "BridgeHardwareVertexBuffer::unlockImpl");
        }

        mIsLocked = false;
    }

    void BridgeHardwareVertexBuffer::readData(size_t offset, size_t length, void* pDest)
    {
        if (mUseShadowBuffer)
        {
            void* srcData = mShadowBuffer->lock(offset, length, HBL_READ_ONLY);
            memcpy(pDest, srcData, length);
            mShadowBuffer->unlock();
        }
        else
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                        "Read hardware buffer is not supported",
                        "BridgeHardwareVertexBuffer::readData");
        }
    }

    void BridgeHardwareVertexBuffer::writeData(size_t offset,
                                           size_t length,
                                           const void* pSource,
                                           bool discardWholeBuffer)
    {
		 // Update the shadow buffer
        if (mUseShadowBuffer)
        {
            void* destData = mShadowBuffer->lock(offset, length,
                                                  discardWholeBuffer ? HBL_DISCARD : HBL_NORMAL);
            memcpy(destData, pSource, length);
            mShadowBuffer->unlock();
        }

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		CHECK_GL_ERROR_DEBUG();
        if (offset == 0 && length == mSizeInBytes)
        {
            glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, pSource,
                         getGLUsageByHardwareUsage(mUsage));
            CHECK_GL_ERROR_DEBUG();
        }
        else
        {
            if(discardWholeBuffer)
            {
                glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, NULL, 
                                getGLUsageByHardwareUsage(mUsage));
                CHECK_GL_ERROR_DEBUG();
            }

            glBufferSubData(GL_ARRAY_BUFFER, offset, length, pSource);
            CHECK_GL_ERROR_DEBUG();
        }

		glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BridgeHardwareVertexBuffer::_updateFromShadow(void)
    {
		if (mUseShadowBuffer && mShadowUpdated && !mSuppressHardwareUpdate)
        {
			const void *srcData = mShadowBuffer->lock(mLockStart, mLockSize,HBL_READ_ONLY);

			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			CHECK_GL_ERROR_DEBUG();
                
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)mSizeInBytes, srcData,
                             getGLUsageByHardwareUsage(mUsage));
            CHECK_GL_ERROR_DEBUG();
			glBindBuffer(GL_ARRAY_BUFFER, 0);

            mShadowBuffer->unlock();
            mShadowUpdated = false;
        }
    }

	BridgeHardwareIndexBuffer::BridgeHardwareIndexBuffer(HardwareBufferManagerBase* mgr, 
													 IndexType idxType,
                                                     size_t numIndexes,
                                                     HardwareBuffer::Usage usage,
                                                     bool useShadowBuffer)
        : HardwareIndexBuffer(mgr, idxType, numIndexes, usage, false, useShadowBuffer)
    {
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		   
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		auto callBack = [this](cocos2d::EventCustom* event)
		{
			this->mShadowUpdated = true;
			this->_updateFromShadow();
		};
		_recreateVBOEventListener = cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_RENDERER_RECREATED, callBack);
#endif
    }

    BridgeHardwareIndexBuffer::~BridgeHardwareIndexBuffer()
    {
  		if(glIsBuffer(_vbo))
		{
			glDeleteBuffers(1, &_vbo);
			_vbo = 0;
		}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(_recreateVBOEventListener);
#endif
    }

    void BridgeHardwareIndexBuffer::unlockImpl(void)
    {
        if (mLockedToScratch)
        {
            if (mScratchUploadOnUnlock)
            {
                    // have to write the data back to vertex buffer
                    writeData(mScratchOffset, mScratchSize, mScratchPtr,
                              mScratchOffset == 0 && mScratchSize == getSizeInBytes());
            }

            static_cast<BridgeHardwareBufferManager*>(
                    HardwareBufferManager::getSingletonPtr())->deallocateScratch(mScratchPtr);

            mLockedToScratch = false;
        }
        else
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                        "Lock to scratch is only supported",
                        "BridgeHardwareIndexBuffer::unlockImpl");
        }
        mIsLocked = false;
    }

    void* BridgeHardwareIndexBuffer::lockImpl(size_t offset,
                                            size_t length,
                                            LockOptions options)
    {
        if(mIsLocked)
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                        "Invalid attempt to lock an index buffer that has already been locked",
                        "BridgeHardwareIndexBuffer::lock");
        }

        void* retPtr = 0;
		BridgeHardwareBufferManager* glBufManager = static_cast<BridgeHardwareBufferManager*>(HardwareBufferManager::getSingletonPtr());

        if(length < glBufManager->getGLMapBufferThreshold())
        {
            retPtr = glBufManager->allocateScratch((uint32)length);
            if (retPtr)
            {
                mLockedToScratch = true;
                mScratchOffset = offset;
                mScratchSize = length;
                mScratchPtr = retPtr;
                mScratchUploadOnUnlock = (options != HBL_READ_ONLY);

                if (options != HBL_DISCARD)
                {
                    readData(offset, length, retPtr);
                }
            }
        }
        else
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                        "Invalid Buffer lockSize",
                        "BridgeHardwareIndexBuffer::lock");
        }

		mIsLocked = true;
        return retPtr;
    }

    void BridgeHardwareIndexBuffer::readData(size_t offset,
                                           size_t length,
                                           void* pDest)
    {
        if(mUseShadowBuffer)
        {
            // Get data from the shadow buffer
            void* srcData = mShadowBuffer->lock(offset, length, HBL_READ_ONLY);
            memcpy(pDest, srcData, length);
            mShadowBuffer->unlock();
        }
        else
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                        "Reading hardware buffer is not supported",
                        "BridgeHardwareIndexBuffer::readData");
        }
    }

    void BridgeHardwareIndexBuffer::writeData(size_t offset, size_t length,
                                            const void* pSource,
                                            bool discardWholeBuffer)
    {
        // Update the shadow buffer
        if (mUseShadowBuffer)
        {
            void* destData = mShadowBuffer->lock(offset, length,
                                                  discardWholeBuffer ? HBL_DISCARD : HBL_NORMAL);
            memcpy(destData, pSource, length);
            mShadowBuffer->unlock();
        }

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		CHECK_GL_ERROR_DEBUG();
        if (offset == 0 && length == mSizeInBytes)
        {
            glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, pSource,
                         getGLUsageByHardwareUsage(mUsage));
            CHECK_GL_ERROR_DEBUG();
        }
        else
        {
            if(discardWholeBuffer)
            {
                glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, NULL, 
                                getGLUsageByHardwareUsage(mUsage));
                CHECK_GL_ERROR_DEBUG();
            }

            glBufferSubData(GL_ARRAY_BUFFER, offset, length, pSource);
            CHECK_GL_ERROR_DEBUG();
        }

		glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BridgeHardwareIndexBuffer::_updateFromShadow(void)
    {
        if (mUseShadowBuffer && mShadowUpdated && !mSuppressHardwareUpdate)
        {

			const void *srcData = mShadowBuffer->lock(mLockStart, mLockSize,HBL_READ_ONLY);

			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			CHECK_GL_ERROR_DEBUG();
                
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)mSizeInBytes, srcData,
                             getGLUsageByHardwareUsage(mUsage));
            CHECK_GL_ERROR_DEBUG();
			glBindBuffer(GL_ARRAY_BUFFER, 0);

            mShadowBuffer->unlock();
            mShadowUpdated = false;
        }
    }


	   // Scratch pool management (32 bit structure)
    struct BridgeScratchBufferAlloc
    {
        /// Size in bytes
        uint32 size: 31;
        /// Free? (pack with size)
        uint32 free: 1;
    };
    #define SCRATCH_POOL_SIZE 1 * 1024 * 1024
    #define SCRATCH_ALIGNMENT 32

    BridgeHardwareBufferManagerBase::BridgeHardwareBufferManagerBase()
    {
        // Init scratch pool
        // TODO make it a configurable size?
        // 32-bit aligned buffer
        mScratchBufferPool = static_cast<char*>(OGRE_MALLOC_ALIGN(SCRATCH_POOL_SIZE,
                                                                  MEMCATEGORY_GEOMETRY,
                                                                  SCRATCH_ALIGNMENT));
        BridgeScratchBufferAlloc* ptrAlloc = (BridgeScratchBufferAlloc*)mScratchBufferPool;
        ptrAlloc->size = SCRATCH_POOL_SIZE - sizeof(BridgeScratchBufferAlloc);
        ptrAlloc->free = 1;

        // non-Win32 machines are having issues glBufferSubData, looks like buffer corruption
		// disable for now until we figure out where the problem lies			
#	if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
		mMapBufferThreshold = 0;
#	endif
}

    BridgeHardwareBufferManagerBase::~BridgeHardwareBufferManagerBase()
    {
        destroyAllDeclarations();
        destroyAllBindings();

        OGRE_FREE_ALIGN(mScratchBufferPool, MEMCATEGORY_GEOMETRY, SCRATCH_ALIGNMENT);
    }

    HardwareVertexBufferSharedPtr
        BridgeHardwareBufferManagerBase::createVertexBuffer(size_t vertexSize,
                                                      size_t numVerts,
                                                      HardwareBuffer::Usage usage,
                                                      bool useShadowBuffer)
    {
        // always use shadowBuffer
        BridgeHardwareVertexBuffer* buf =
            OGRE_NEW BridgeHardwareVertexBuffer(this, vertexSize, numVerts, usage, true);
        {
            OGRE_LOCK_MUTEX(mVertexBuffersMutex)
            mVertexBuffers.insert(buf);
        }
        return HardwareVertexBufferSharedPtr(buf);
    }

    HardwareIndexBufferSharedPtr BridgeHardwareBufferManagerBase::createIndexBuffer(HardwareIndexBuffer::IndexType itype,
                                                                              size_t numIndexes,
                                                                              HardwareBuffer::Usage usage,
                                                                              bool useShadowBuffer)
    {
        // always use shadowBuffer
        BridgeHardwareIndexBuffer* buf =
            OGRE_NEW BridgeHardwareIndexBuffer(this, itype, numIndexes, usage, true);
        {
            OGRE_LOCK_MUTEX(mIndexBuffersMutex)
            mIndexBuffers.insert(buf);
        }
        return HardwareIndexBufferSharedPtr(buf);
    }

	RenderToVertexBufferSharedPtr BridgeHardwareBufferManagerBase::createRenderToVertexBuffer()
	{
		// not supported
		return RenderToVertexBufferSharedPtr();
	}

    void* BridgeHardwareBufferManagerBase::allocateScratch(uint32 size)
    {
        // simple forward link search based on alloc sizes
        // not that fast but the list should never get that long since not many
        // locks at once (hopefully)
        OGRE_LOCK_MUTEX(mScratchMutex)

        // Alignment - round up the size to 32 bits
        // control blocks are 32 bits too so this packs nicely
        if (size % 4 != 0)
        {
            size += 4 - (size % 4);
        }

        uint32 bufferPos = 0;
        while (bufferPos < SCRATCH_POOL_SIZE)
        {
            BridgeScratchBufferAlloc* pNext = (BridgeScratchBufferAlloc*)(mScratchBufferPool + bufferPos);
            // Big enough?
            if (pNext->free && pNext->size >= size)
            {
                // split? And enough space for control block
                if(pNext->size > size + sizeof(BridgeScratchBufferAlloc))
                {
                    uint32 offset = sizeof(BridgeScratchBufferAlloc) + size;

                    BridgeScratchBufferAlloc* pSplitAlloc = (BridgeScratchBufferAlloc*)
                        (mScratchBufferPool + bufferPos + offset);
                    pSplitAlloc->free = 1;
                    // split size is remainder minus new control block
                    pSplitAlloc->size = pNext->size - size - sizeof(BridgeScratchBufferAlloc);

                    // New size of current
                    pNext->size = size;
                }
                // allocate and return
                pNext->free = 0;

                // return pointer just after this control block (++ will do that for us)
                return ++pNext;
            }

            bufferPos += sizeof(BridgeScratchBufferAlloc) + pNext->size;
        }

        // no available alloc
        return 0;
    }

    void BridgeHardwareBufferManagerBase::deallocateScratch(void* ptr)
    {
        OGRE_LOCK_MUTEX(mScratchMutex)

        // Simple linear search dealloc
        uint32 bufferPos = 0;
        BridgeScratchBufferAlloc* pLast = 0;
        while (bufferPos < SCRATCH_POOL_SIZE)
        {
            BridgeScratchBufferAlloc* pCurrent = (BridgeScratchBufferAlloc*)(mScratchBufferPool + bufferPos);

            // Pointers match?
            if ((mScratchBufferPool + bufferPos + sizeof(BridgeScratchBufferAlloc)) == ptr)
            {
                // dealloc
                pCurrent->free = 1;
                // merge with previous
                if (pLast && pLast->free)
                {
                    // adjust buffer pos
                    bufferPos -= (pLast->size + sizeof(BridgeScratchBufferAlloc));
                    // merge free space
                    pLast->size += pCurrent->size + sizeof(BridgeScratchBufferAlloc);
                    pCurrent = pLast;
                }

                // merge with next
                uint32 offset = bufferPos + pCurrent->size + sizeof(BridgeScratchBufferAlloc);
                if (offset < SCRATCH_POOL_SIZE)
                {
                    BridgeScratchBufferAlloc* pNext = (BridgeScratchBufferAlloc*)(
                        mScratchBufferPool + offset);
                    if (pNext->free)
                    {
                        pCurrent->size += pNext->size + sizeof(BridgeScratchBufferAlloc);
                    }
                }

                // done
                return;
            }

            bufferPos += sizeof(BridgeScratchBufferAlloc) + pCurrent->size;
            pLast = pCurrent;

        }

        // Should never get here unless there's a corruption
        assert(false && "Memory deallocation error");
    }
	//---------------------------------------------------------------------
	size_t BridgeHardwareBufferManagerBase::getGLMapBufferThreshold() const
	{
		return mMapBufferThreshold;
	}
	//---------------------------------------------------------------------
	void BridgeHardwareBufferManagerBase::setGLMapBufferThreshold( const size_t value )
	{
		mMapBufferThreshold = value;
	}
}
