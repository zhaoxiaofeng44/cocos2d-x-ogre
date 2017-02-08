/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2012 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "OgreBridgeTextureManager.h"

#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreHardwarePixelBuffer.h"

#include "renderer/CCVertexIndexBuffer.h"
#include "base/CCEventType.h"
#include "base/CCEventListenerCustom.h"
#include "base/CCEventDispatcher.h"
#include "base/CCDirector.h"

#include "2d/CCSpriteFrameCache.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCTexture2D.h"
#include "renderer/CCRenderer.h"
#include "base/CCDirector.h"
namespace Ogre {

	static inline Ogre::PixelFormat covertPixFormat(cocos2d::Texture2D::PixelFormat format)
    {
		Ogre::PixelFormat result;
		switch (format)
		{
			//! 32-bit texture: BGRA8888
		case cocos2d::Texture2D::PixelFormat::BGRA8888:
			result = PF_A8B8G8R8;
			break;
			//! 32-bit texture: RGBA8888
		case cocos2d::Texture2D::PixelFormat::RGBA8888:
			result = PF_A8R8G8B8;
			break;
			//! 24-bit texture: RGBA888
		case cocos2d::Texture2D::PixelFormat::RGB888:
			result = PF_R8G8B8;
			break;
			//! 16-bit texture without Alpha channel
		case cocos2d::Texture2D::PixelFormat::RGB565:
			result = PF_R5G6B5;
			break;
			//! 8-bit textures used as masks
		case cocos2d::Texture2D::PixelFormat::A8:
			result = PF_A8;
			break;
			//! 8-bit intensity texture
		case cocos2d::Texture2D::PixelFormat::I8:
			result = PF_L8;
			break;
			//! 16-bit textures used as masks
		case cocos2d::Texture2D::PixelFormat::AI88:
			result = PF_BYTE_LA;
			break;
			//! 16-bit textures: RGBA4444
		case cocos2d::Texture2D::PixelFormat::RGBA4444:
			result = PF_A4R4G4B4;
			break;
			//! 16-bit textures: RGB5A1
		case cocos2d::Texture2D::PixelFormat::RGB5A1:
			result = PF_A1R5G5B5;
			break;
			//! 4-bit PVRTC-compressed texture: PVRTC4
		case cocos2d::Texture2D::PixelFormat::PVRTC4:
			result = PF_PVRTC_RGB4;
			break;
			//! 4-bit PVRTC-compressed texture: PVRTC4 (has alpha channel)
		case cocos2d::Texture2D::PixelFormat::PVRTC4A:
			result = PF_PVRTC_RGBA4;
			break;
			//! 2-bit PVRTC-compressed texture: PVRTC2
		case cocos2d::Texture2D::PixelFormat::PVRTC2:
			result = PF_PVRTC_RGB2;
			break;
			//! 2-bit PVRTC-compressed texture: PVRTC2 (has alpha channel)
		case cocos2d::Texture2D::PixelFormat::PVRTC2A:
			result = PF_PVRTC_RGBA2;
			break;
			//! ETC-compressed texture: ETC
		case cocos2d::Texture2D::PixelFormat::ETC:
			//! S3TC-compressed texture: S3TC_Dxt1
		case cocos2d::Texture2D::PixelFormat::S3TC_DXT1:
			//! S3TC-compressed texture: S3TC_Dxt3
		case cocos2d::Texture2D::PixelFormat::S3TC_DXT3:
			//! S3TC-compressed texture: S3TC_Dxt5
		case cocos2d::Texture2D::PixelFormat::S3TC_DXT5:
			//! ATITC-compressed texture: ATC_RGB
		case cocos2d::Texture2D::PixelFormat::ATC_RGB:
			//! ATITC-compressed texture: ATC_EXPLICIT_ALPHA
		case cocos2d::Texture2D::PixelFormat::ATC_EXPLICIT_ALPHA:
			//! ATITC-compresed texture: ATC_INTERPOLATED_ALPHA
		case cocos2d::Texture2D::PixelFormat::ATC_INTERPOLATED_ALPHA:
			//! Default texture format: AUTO
		default: // Texture2D::PixelFormat::DEFAULT = AUTO:
			result = PF_UNKNOWN;
			break;
		};
		return result;
    }

    static inline GLenum getBridgeTextureTarget(TextureType mTextureType)
    {
        switch(mTextureType)
        {
            case TEX_TYPE_1D:
            case TEX_TYPE_2D:
                return GL_TEXTURE_2D;
            case TEX_TYPE_CUBE_MAP:
                return GL_TEXTURE_CUBE_MAP;
            default:
                return 0;
        };
    }

    BridgeTexture::BridgeTexture(ResourceManager* creator, const String& name,
                             ResourceHandle handle, const String& group, bool isManual,
                             ManualResourceLoader* loader)
        : Texture(creator, name, handle, group, isManual, loader),
          mTextureID(0)
    {
    }

    BridgeTexture::~BridgeTexture()
    {
        // have to call this here rather than in Resource destructor
        // since calling virtual methods in base destructors causes crash
        /*if (isLoaded())
        {
            unload();
        }
        else
        {
            freeInternalResources();
        }*/
    }

    // Creation / loading methods
    void BridgeTexture::createInternalResourcesImpl(void)
    {

		if(!mCocosTextureList.empty())
		{
			mTextureID = mCocosTextureList[0]->getName();
		}
	}	
 
    void BridgeTexture::createRenderTexture(void)
    {
        // Create the GL texture
        // This already does everything necessary
        createInternalResources();
    }

	static unsigned char cc_2x2_white_image[] = {
		// RGBA8888
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF
	};

	#define CC_2x2_WHITE_IMAGE_KEY  "/cc_2x2_white_image"

    void BridgeTexture::prepareImpl()
    {
        if (mUsage & TU_RENDERTARGET) return;

        if (mTextureType == TEX_TYPE_1D || mTextureType == TEX_TYPE_2D)
        {
			bool bRet = false;
			cocos2d::Texture2D *texture = nullptr;
			do
			{
				texture = cocos2d::Director::getInstance()->getTextureCache()->getTextureForKey(mName);
				CC_BREAK_IF(nullptr != texture);

				std::string fullpath = cocos2d::FileUtils::getInstance()->fullPathForFilename(mName);
				CC_BREAK_IF(fullpath.empty());

				cocos2d::Image* image = new (std::nothrow) cocos2d::Image();
				CC_BREAK_IF(nullptr == image);

				bRet = image->initWithImageFile(fullpath);
				CC_BREAK_IF(!bRet);

				texture = new (std::nothrow) cocos2d::Texture2D();
				texture->initWithImage(image,cocos2d::Texture2D::PixelFormat::RGBA8888);//texture->initWithData(image->getData(),image->getDataLen(),cocos2d::Texture2D::PixelFormat::BGRA8888,image->getWidth(),image->getHeight(),cocos2d::Size(image->getWidth(),image->getHeight()));
				CC_SAFE_RELEASE(image);

			}while(0);

			if(!texture)
			{
				texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(CC_2x2_WHITE_IMAGE_KEY);
				if(nullptr == texture)
				{
					cocos2d::Image* image = new (std::nothrow) cocos2d::Image();
					bool isOK = image->initWithRawData(cc_2x2_white_image, sizeof(cc_2x2_white_image), 2, 2, 8);
					CC_UNUSED_PARAM(isOK);
					CCASSERT(isOK, "The 2x2 empty texture was created unsuccessfully.");
					texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(image,CC_2x2_WHITE_IMAGE_KEY);
					CC_SAFE_RELEASE(image);
				}
			}
			mTextureID = texture->getName();
			mCocosTextureList.push_back(texture);	

			mDepth = 1;
			mNumMipmaps = 0;
			mWidth = texture->getContentSize().width;
			mHeight = texture->getContentSize().height;
			mFormat = covertPixFormat(texture->getPixelFormat());
		}
    }

    void BridgeTexture::unprepareImpl()
    {
        mLoadedImages.setNull();
    }

    void BridgeTexture::loadImpl()
    {
        if (mUsage & TU_RENDERTARGET)
        {
            createRenderTexture();
            return;
        }
		/*
        // Now the only copy is on the stack and will be cleaned in case of
        // exceptions being thrown from _loadImages
        LoadedImages loadedImages = mLoadedImages;
        mLoadedImages.setNull();

        // Call internal _loadImages, not loadImage since that's external and 
        // will determine load status etc again
        ConstImagePtrList imagePtrs;

        for (size_t i = 0; i < loadedImages->size(); ++i)
        {
            imagePtrs.push_back(&(*loadedImages)[i]);
        }

        _loadImages(imagePtrs);*/
    }

    void BridgeTexture::freeInternalResourcesImpl()
    {
       /* mSurfaceList.clear();
        glDeleteTextures(1, &mTextureID);
        CHECK_GL_ERROR_DEBUG();*/
    }

    void BridgeTexture::_createSurfaceList()
    {
        /*mSurfaceList.clear();

        // For all faces and mipmaps, store surfaces as HardwarePixelBufferSharedPtr
        bool wantGeneratedMips = (mUsage & TU_AUTOMIPMAP)!=0;

        // Do mipmapping in software? (uses GLU) For some cards, this is still needed. Of course,
        // only when mipmap generation is desired.
        bool doSoftware = wantGeneratedMips && !mMipmapsHardwareGenerated && getNumMipmaps();

        for (size_t face = 0; face < getNumFaces(); face++)
        {
			size_t width = mWidth;
			size_t height = mHeight;

            for (size_t mip = 0; mip <= getNumMipmaps(); mip++)
            {
                GLES2HardwarePixelBuffer *buf = OGRE_NEW BridgeTextureBuffer(mName,
                                                                            getBridgeTextureTarget(),
                                                                            mTextureID,
                                                                            width, height,
                                                                            GLES2PixelUtil::getClosestGLInternalFormat(mFormat, mHwGamma),
                                                                            GLES2PixelUtil::getGLOriginDataType(mFormat),
                                                                            face,
                                                                            mip,
                                                                            static_cast<HardwareBuffer::Usage>(mUsage),
                                                                            doSoftware && mip==0, mHwGamma, mFSAA);

                mSurfaceList.push_back(HardwarePixelBufferSharedPtr(buf));

                // Check for error
                if (buf->getWidth() == 0 ||
                    buf->getHeight() == 0 ||
                    buf->getDepth() == 0)
                {
                    OGRE_EXCEPT(
                        Exception::ERR_RENDERINGAPI_ERROR,
                        "Zero sized texture surface on texture "+getName()+
                            " face "+StringConverter::toString(face)+
                            " mipmap "+StringConverter::toString(mip)+
                            ". The GL driver probably refused to create the texture.",
                            "BridgeTexture::_createSurfaceList");
                }
            }
        }*/
    }

    HardwarePixelBufferSharedPtr BridgeTexture::getBuffer(size_t face, size_t mipmap)
    {
        if (face >= getNumFaces())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                        "Face index out of range",
                        "BridgeTexture::getBuffer");
        }

        if (mipmap > mNumMipmaps)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                        "Mipmap index out of range",
                        "BridgeTexture::getBuffer");
        }

		 OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                        "Mipmap index out of range",
                        "BridgeTexture::getBuffer");

    
        return HardwarePixelBufferSharedPtr();
    }






    BridgeHardwareTextureManager::BridgeHardwareTextureManager()
        : TextureManager()
    {        
        // Register with group manager
        ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
    }

    BridgeHardwareTextureManager::~BridgeHardwareTextureManager()
    {
        // Unregister with group manager
        ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
    }

    Resource* BridgeHardwareTextureManager::createImpl(const String& name, ResourceHandle handle, 
                                           const String& group, bool isManual,
                                           ManualResourceLoader* loader,
                                           const NameValuePairList* createParams)
    {
         return OGRE_NEW BridgeTexture(this, name, handle, group, isManual, loader);
    }

    //-----------------------------------------------------------------------------
    void BridgeHardwareTextureManager::createWarningTexture()
    {
		MaterialManager::getSingleton().create(
                "BaseWhite",
                ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);


		//MaterialManager::getSingleton().create("W"); "BaseWhite"ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME
    }

    PixelFormat BridgeHardwareTextureManager::getNativeFormat(TextureType ttype, PixelFormat format, int usage)
    {
        // Adjust requested parameters to capabilities
        const RenderSystemCapabilities *caps = Root::getSingleton().getRenderSystem()->getCapabilities();

        // Check compressed texture support
        // if a compressed format not supported, revert to PF_A8R8G8B8
        if (PixelUtil::isCompressed(format) &&
            !caps->hasCapability(RSC_TEXTURE_COMPRESSION_DXT) && !caps->hasCapability(RSC_TEXTURE_COMPRESSION_PVRTC))
        {
            return PF_A8R8G8B8;
        }
        // if floating point textures not supported, revert to PF_A8R8G8B8
        if (PixelUtil::isFloatingPoint(format) &&
            !caps->hasCapability(RSC_TEXTURE_FLOAT))
        {
            return PF_A8R8G8B8;
        }

        // Check if this is a valid rendertarget format
        if (usage & TU_RENDERTARGET)
        {
            /// Get closest supported alternative
            /// If mFormat is supported it's returned
            //return GLES2RTTManager::getSingleton().getSupportedAlternative(format);
        }

        // Supported
        return format;
    }

    bool BridgeHardwareTextureManager::isHardwareFilteringSupported(TextureType ttype, PixelFormat format, int usage,
            bool preciseFormatOnly)
    {
        if (format == PF_UNKNOWN)
        {
            return false;
        }

        // Check native format
        PixelFormat nativeFormat = getNativeFormat(ttype, format, usage);
        if (preciseFormatOnly && format != nativeFormat)
        {
            return false;
        }

        // Assume non-floating point is supported always
        if (!PixelUtil::isFloatingPoint(nativeFormat))
        {
            return true;
        }

        return false;
    }
}
