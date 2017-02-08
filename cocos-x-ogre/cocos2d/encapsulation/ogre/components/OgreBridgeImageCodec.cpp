#include "OgreBridgeImageCodec.h"

#include <OgreCodec.h>
#include <renderer/CCTexture2D.h>

namespace Ogre {

	class BridgeImageStream : public cocos2d::Image
	{
	private:
		Format mImageType;
	public:
		BridgeImageStream(const Format &fiType) :
			mImageType(fiType)
		{}

		virtual ~BridgeImageStream(){}

		bool decode(DataStreamPtr& input)
		{
			MemoryDataStream memStream(input, true);
			ssize_t size = memStream.size();
			const unsigned char *data = memStream.getPtr();

			bool ret = false;
			switch (mImageType)
			{
			case Format::PNG:
				ret = initWithPngData(data, size);
				break;
			case Format::JPG:
				ret = initWithJpgData(data, size);
				break;
			case Format::TIFF:
				ret = initWithTiffData(data, size);
				break;
			case Format::WEBP:
				ret = initWithWebpData(data, size);
				break;
			case Format::PVR:
				ret = initWithPVRData(data, size);
				break;
			case Format::ETC:
				ret = initWithETCData(data, size);
				break;
			case Format::S3TC:
				ret = initWithS3TCData(data, size);
				break;
			case Format::ATITC:
				ret = initWithATITCData(data, size);
				break;
			default:
			{
					   /*  // load and detect image format
					   tImageTGA* tgaData = tgaLoadBuffer(data, size);

					   if (tgaData != nullptr && tgaData->status == TGA_OK)
					   {
					   ret = initWithTGAData(tgaData);
					   }
					   else
					   {
					   CCAssert(false, "unsupport image format!");
					   }

					   free(tgaData);
					   break;*/
			}
			}

			return ret;

		}
       
		bool useImageData(ImageCodec::ImageData* imgData)
		{
			bool ret = true;
       
			switch (getRenderFormat())
			{
				//! 32-bit texture: BGRA8888
			case cocos2d::Texture2D::PixelFormat::BGRA8888:
				imgData->format = PF_A8B8G8R8;
				break;
				//! 32-bit texture: RGBA8888
			case cocos2d::Texture2D::PixelFormat::RGBA8888:
				imgData->format = PF_A8R8G8B8;
				break;
				//! 24-bit texture: RGBA888
			case cocos2d::Texture2D::PixelFormat::RGB888:
				imgData->format = PF_R8G8B8;
				break;
				//! 16-bit texture without Alpha channel
			case cocos2d::Texture2D::PixelFormat::RGB565:
				imgData->format = PF_R5G6B5;
				break;
				//! 8-bit textures used as masks
			case cocos2d::Texture2D::PixelFormat::A8:
				imgData->format = PF_A8;
				break;
				//! 8-bit intensity texture
			case cocos2d::Texture2D::PixelFormat::I8:
				imgData->format = PF_L8;
				break;
				//! 16-bit textures used as masks
			case cocos2d::Texture2D::PixelFormat::AI88:
				imgData->format = PF_BYTE_LA;
				break;
				//! 16-bit textures: RGBA4444
			case cocos2d::Texture2D::PixelFormat::RGBA4444:
				imgData->format = PF_A4R4G4B4;
				break;
				//! 16-bit textures: RGB5A1
			case cocos2d::Texture2D::PixelFormat::RGB5A1:
				imgData->format = PF_A1R5G5B5;
				break;
				//! 4-bit PVRTC-compressed texture: PVRTC4
			case cocos2d::Texture2D::PixelFormat::PVRTC4:
				imgData->format = PF_PVRTC_RGB4;
				break;
				//! 4-bit PVRTC-compressed texture: PVRTC4 (has alpha channel)
			case cocos2d::Texture2D::PixelFormat::PVRTC4A:
				imgData->format = PF_PVRTC_RGBA4;
				break;
				//! 2-bit PVRTC-compressed texture: PVRTC2
			case cocos2d::Texture2D::PixelFormat::PVRTC2:
				imgData->format = PF_PVRTC_RGB2;
				break;
				//! 2-bit PVRTC-compressed texture: PVRTC2 (has alpha channel)
			case cocos2d::Texture2D::PixelFormat::PVRTC2A:
				imgData->format = PF_PVRTC_RGBA2;
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
				imgData->format = PF_UNKNOWN;
				ret = false;
				break;
			};

			if (ret)
			{
				imgData->depth = 1; // only 2D formats handled by this codec
				imgData->width = getWidth();
				imgData->height = getHeight();
				imgData->num_mipmaps = 0; // no mipmaps in non-DDS 
				imgData->flags = 0;
				imgData->size = getDataLen();
			}
			return ret;
		}

		bool useStreamImagePtr(MemoryDataStreamPtr &output)
		{
			bool ret = true;
			if (this->getDataLen() > 0)
			{
				output.bind(OGRE_NEW MemoryDataStream(this->getDataLen()));
				uchar* pDst = output->getPtr();
				uchar* pSrc = this->getData();
				memcpy(pDst, pSrc, this->getDataLen());
			}
			else
				ret = false;

			return ret;
		}
	};

	static std::vector<ImageCodec * > s_sCodecList;


	/// @copydoc Codec::decode
	Codec::DecodeResult BridgeImageCodec::decode(DataStreamPtr& input) const
	{
		BridgeImageStream mImageStream(mImageFitype);
		bool ret = true;
		if (ret)
		{
			ret = mImageStream.decode(input);
		}

		ImageData* imgData = NULL;
		if (ret)
		{
			imgData = OGRE_NEW ImageData();
			ret = mImageStream.useImageData(imgData);
		}

		MemoryDataStreamPtr output;
		if (ret)
		{
			ret = mImageStream.useStreamImagePtr(output);
		}

		DecodeResult result;
		if (ret)
		{
			result.first = output;
			result.second = CodecDataPtr(imgData);
		}

		return result;
	}

		/// Static method to startup FreeImage and register the FreeImage codecs
	void BridgeImageCodec::startup(void)
	{
		static const int supports_num = 8;
		static const String supports[] = { "png", "jpg", "tiff", "webp", "pvr", "etc", "s3tc", "atitc" };
		static const cocos2d::Image::Format supports_type[] = { cocos2d::Image::Format::PNG, cocos2d::Image::Format::JPG, cocos2d::Image::Format::TIFF, cocos2d::Image::Format::WEBP, cocos2d::Image::Format::PVR, cocos2d::Image::Format::ETC, cocos2d::Image::Format::S3TC, cocos2d::Image::Format::ATITC };

		BridgeImageCodec* codec = NULL;
		for (int i = 0; i < supports_num; i++)
		{
			codec = OGRE_NEW BridgeImageCodec(supports[i], supports_type[i]);
			s_sCodecList.push_back(codec);
			Codec::registerCodec(codec);
		}
	}

		/// Static method to shutdown FreeImage and unregister the FreeImage codecs
	void BridgeImageCodec::shutdown(void)
	{
		std::vector<ImageCodec*>::iterator itor, end;

		end = s_sCodecList.end();
		itor = s_sCodecList.begin();

		for (; itor != end; itor++)
		{
			Codec::unRegisterCodec(*itor);
			OGRE_DELETE *itor;
		}
		s_sCodecList.clear();
	}
};
