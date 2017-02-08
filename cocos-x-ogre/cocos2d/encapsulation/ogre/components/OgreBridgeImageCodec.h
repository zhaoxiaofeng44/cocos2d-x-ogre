#ifndef OGRE_BRIDGE_IMAGE_CODEC_H
#define OGRE_BRIDGE_IMAGE_CODEC_H

#include "cocos2d.h"

#include <OgreCodec.h>
#include <OgreImageCodec.h>

namespace Ogre {

	class BridgeImageCodec : public ImageCodec
	{
	private:
		String mType;
		cocos2d::Image::Format mImageFitype;

	public:
		BridgeImageCodec(const String &type, const cocos2d::Image::Format &fiType) :
			mType(type),
			mImageFitype(fiType)
		{}

		virtual ~BridgeImageCodec(){}

		/// @copydoc Codec::code
		DataStreamPtr code(MemoryDataStreamPtr& input, CodecDataPtr& pData) const{return DataStreamPtr();}

		/// @copydoc Codec::codeToFile
		void codeToFile(MemoryDataStreamPtr& input, const String& outFileName, CodecDataPtr& pData) const{}

		/// @copydoc Codec::decode
		virtual Codec::DecodeResult decode(DataStreamPtr& input) const;

		virtual String getType() const{ return mType; }

		/// @copydoc Codec::magicNumberToFileExt
		String magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const{return "";}

		/// Static method to startup FreeImage and register the FreeImage codecs
		static void startup(void);

		/// Static method to shutdown FreeImage and unregister the FreeImage codecs
		static void shutdown(void);
	};

}
#endif//OGREAPP_H
