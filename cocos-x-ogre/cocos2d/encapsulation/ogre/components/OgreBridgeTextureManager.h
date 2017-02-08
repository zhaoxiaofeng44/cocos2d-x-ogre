#ifndef OGRE_BRIDGE_TEXTURE_MANAGER_H
#define OGRE_BRIDGE_TEXTURE_MANAGER_H

#include "cocos2d.h"

#include "OgreTexture.h"
#include "OgreTextureManager.h"

NS_CC_BEGIN

	class Texture2D;
NS_CC_END


namespace Ogre {

	class BridgeTexture : public Texture
    {
        public:
            // Constructor
            BridgeTexture(ResourceManager* creator, const String& name, ResourceHandle handle,
                const String& group, bool isManual, ManualResourceLoader* loader);

            virtual ~BridgeTexture();

            void createRenderTexture();
            /// @copydoc Texture::getBuffer
            HardwarePixelBufferSharedPtr getBuffer(size_t face, size_t mipmap);

            // Takes the OGRE texture type (1d/2d/3d/cube) and returns the appropriate GL one
            GLenum getBridgeTextureTarget(void) const;

            GLuint getGLID() const{ return mTextureID;}

        protected:
            /// @copydoc Texture::createInternalResourcesImpl
            void createInternalResourcesImpl(void);
            /// @copydoc Resource::prepareImpl
            void prepareImpl(void);
            /// @copydoc Resource::unprepareImpl
            void unprepareImpl(void);
            /// @copydoc Resource::loadImpl
            void loadImpl(void);
            /// @copydoc Resource::freeInternalResourcesImpl
            void freeInternalResourcesImpl(void);

            /** Internal method, create GLHardwarePixelBuffers for every face and
             mipmap level. This method must be called after the GL texture object was created,
             the number of mipmaps was set (GL_TEXTURE_MAX_LEVEL) and glTexImageXD was called to
             actually allocate the buffer
             */
            void _createSurfaceList();

            /// Used to hold images between calls to prepare and load.
            typedef SharedPtr<vector<Image>::type > LoadedImages;

            /** Vector of images that were pulled from disk by
             prepareLoad but have yet to be pushed into texture memory
             by loadImpl.  Images should be deleted by loadImpl and unprepareImpl.
             */
            LoadedImages mLoadedImages;


        private:
            GLuint mTextureID;
            
			typedef vector<cocos2d::Texture2D *>::type CocosTextureList;
			CocosTextureList mCocosTextureList;
    };


	   /** Specialisation of SharedPtr to allow SharedPtr to be assigned to GLES2TexturePtr
    @note Has to be a subclass since we need operator=.
    We could templatise this instead of repeating per Resource subclass,
    except to do so requires a form VC6 does not support i.e.
    ResourceSubclassPtr<T> : public SharedPtr<T>
    */
    class BridgeTexturePtr : public SharedPtr<BridgeTexture>
    {
        public:
            BridgeTexturePtr() : SharedPtr<BridgeTexture>() {}
            explicit BridgeTexturePtr(BridgeTexture* rep) : SharedPtr<BridgeTexture>(rep) {}
            BridgeTexturePtr(const BridgeTexturePtr& r) : SharedPtr<BridgeTexture>(r) {}

            BridgeTexturePtr(const ResourcePtr& r) : SharedPtr<BridgeTexture>()
            {
                // lock & copy other mutex pointer
                OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
                {
                    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                    pRep = static_cast<BridgeTexture*>(r.getPointer());
                    pUseCount = r.useCountPointer();
                    if (pUseCount)
                    {
                        ++(*pUseCount);
                    }
                }
            }

            BridgeTexturePtr(const TexturePtr& r) : SharedPtr<BridgeTexture>()
            {
                *this = r;
            }

            /// Operator used to convert a ResourcePtr to a GLESTexturePtr
            BridgeTexturePtr& operator=(const ResourcePtr& r)
            {
                if (pRep == static_cast<BridgeTexture*>(r.getPointer()))
                {
                    return *this;
                }
                release();
                // lock & copy other mutex pointer
                OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
                {
                    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                    pRep = static_cast<BridgeTexture*>(r.getPointer());
                    pUseCount = r.useCountPointer();
                    if (pUseCount)
                    {
                        ++(*pUseCount);
                    }
                }
                else
                {
                    // RHS must be a null pointer
                    assert(r.isNull() && "RHS must be null if it has no mutex!");
                    setNull();
                }
                return *this;
            }

            /// Operator used to convert a TexturePtr to a GLESTexturePtr
            BridgeTexturePtr& operator=(const TexturePtr& r)
            {
                if (pRep == static_cast<BridgeTexture*>(r.getPointer()))
                    return *this;
                release();
                // lock & copy other mutex pointer
                OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
                {
                    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                    pRep = static_cast<BridgeTexture*>(r.getPointer());
                    pUseCount = r.useCountPointer();
                    if (pUseCount)
                    {
                        ++(*pUseCount);
                    }
                }
                else
                {
                    // RHS must be a null pointer
                    assert(r.isNull() && "RHS must be null if it has no mutex!");
                    setNull();
                }
                return *this;
            }
    };

    /** GL ES-specific implementation of a TextureManager */
    class BridgeHardwareTextureManager : public TextureManager
    {
        public:
            BridgeHardwareTextureManager();

            virtual ~BridgeHardwareTextureManager();

			void createWarningTexture();

            GLuint getWarningTextureID() { return mWarningTextureID; }

            /// @copydoc TextureManager::getNativeFormat
            PixelFormat getNativeFormat(TextureType ttype, PixelFormat format, int usage);

            /// @copydoc TextureManager::isHardwareFilteringSupported
            bool isHardwareFilteringSupported(TextureType ttype, PixelFormat format, int usage,
                                              bool preciseFormatOnly = false);
    protected:

        /// @copydoc ResourceManager::createImpl
        Resource* createImpl(const String& name, ResourceHandle handle,
                             const String& group, bool isManual, ManualResourceLoader* loader, 
                             const NameValuePairList* createParams);
        
        /// Internal method to create a warning texture (bound when a texture unit is blank)


        GLuint mWarningTextureID;
    };
}

#endif
