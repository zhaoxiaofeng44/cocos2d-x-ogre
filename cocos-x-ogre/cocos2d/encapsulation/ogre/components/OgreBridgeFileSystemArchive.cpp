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
#include "OgreStableHeaders.h"

#include "OgreLogManager.h"
#include "OgreException.h"
#include "OgreStringVector.h"
#include "OgreRoot.h"

#include "OgreBridgeFileSystemArchive.h"

#include "cocos2d.h"

namespace Ogre {

	bool BridgeFileSystemArchive::msIgnoreHidden = true;

	//-----------------------------------------------------------------------
	BridgeFileSystemArchive::BridgeFileSystemArchive(const String& name, const String& archType)
		: Archive(name, archType)
	{
	}
	//-----------------------------------------------------------------------
	bool BridgeFileSystemArchive::isCaseSensitive(void) const
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		return false;
#else
		return true;
#endif
	}

	//-----------------------------------------------------------------------
	BridgeFileSystemArchive::~BridgeFileSystemArchive()
	{
		unload();
	}
	//-----------------------------------------------------------------------
	void BridgeFileSystemArchive::load()
	{
		mReadOnly = false;
	}
	//-----------------------------------------------------------------------
	void BridgeFileSystemArchive::unload()
	{
		// nothing to see here, move along
	}
	//-----------------------------------------------------------------------
	DataStreamPtr BridgeFileSystemArchive::open(const String& filename, bool readOnly) const
	{

		cocos2d::Data data = cocos2d::FileUtils::getInstance()->getDataFromFile(filename);
		
		MemoryDataStreamPtr streamPtr;
		streamPtr.bind(new MemoryDataStream(data.getSize()));
		uchar* pDst = streamPtr->getPtr();
		memcpy(pDst, data.getBytes(), data.getSize());

		return DataStreamPtr(streamPtr);
	}
	//---------------------------------------------------------------------
	DataStreamPtr BridgeFileSystemArchive::create(const String& filename) const
	{
		/*if (isReadOnly())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Cannot create a file in a read-only archive",
				"FileSystemArchive::remove");
		}

		String full_path = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename);
		// Always open in binary mode

		// Also, always include reading
		std::ios::openmode mode = std::ios::out | std::ios::binary;
		std::fstream* rwStream = OGRE_NEW_T(std::fstream, MEMCATEGORY_GENERAL)();
		rwStream->open(full_path.c_str(), mode);

		// Should check ensure open succeeded, in case fail for some reason.
		if (rwStream->fail())
		{
			OGRE_DELETE_T(rwStream, basic_fstream, MEMCATEGORY_GENERAL);
			OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
				"Cannot open file: " + filename,
				"FileSystemArchive::create");
		}

		/// Construct return stream, tell it to delete on destroy
		FileStreamDataStream* stream = OGRE_NEW FileStreamDataStream(filename,
			rwStream, 0, true);
		*/
		return DataStreamPtr();
	}
	//---------------------------------------------------------------------
	void BridgeFileSystemArchive::remove(const String& filename) const
	{
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
			"Cannot remove a file from a read-only archive",
			"BridgeFileSystemArchive::remove");
	}
	//-----------------------------------------------------------------------
	StringVectorPtr BridgeFileSystemArchive::list(bool recursive, bool dirs)
	{
		// directory change requires locking due to saved returns
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		StringVectorPtr ret(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

		//findFiles("*", recursive, dirs, ret.getPointer(), 0);

		return ret;
	}
	//-----------------------------------------------------------------------
	FileInfoListPtr BridgeFileSystemArchive::listFileInfo(bool recursive, bool dirs)
	{
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		FileInfoListPtr ret(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

		
		return ret;
	}
	//-----------------------------------------------------------------------
	StringVectorPtr BridgeFileSystemArchive::find(const String& pattern,
		bool recursive, bool dirs)
	{
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		StringVectorPtr ret(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);


		return ret;

	}
	//-----------------------------------------------------------------------
	FileInfoListPtr BridgeFileSystemArchive::findFileInfo(const String& pattern,
		bool recursive, bool dirs) const
	{
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		FileInfoListPtr ret(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

		return ret;
	}
	//-----------------------------------------------------------------------
	bool BridgeFileSystemArchive::exists(const String& filename)
	{
		return cocos2d::FileUtils::getInstance()->isFileExist(filename);
	}
	//---------------------------------------------------------------------
	time_t BridgeFileSystemArchive::getModifiedTime(const String& filename)
	{
		return 0;
	}
	//-----------------------------------------------------------------------
	const String& BridgeFileSystemArchiveFactory::getType(void) const
	{
		static String name = "BridgeFileSystem";
		return name;
	}

}
