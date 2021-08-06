#include "FileItem.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/Allocator/DefaultAllocator.h"

#if ALICE_EDITOR
#include <io.h>
#endif

#if ALICE_ANDROID
#elif ALICE_EDITOR
namespace Alice
{
	FileItem*FileItem::Get(const char*path)
	{
		FileItem*item = nullptr;
#if ALICE_EDITOR
		_finddata_t fileinfo;
		long hFile = _findfirst(path, &fileinfo);
		if (hFile)
		{
			item = new (kMemDefaultId)FileItem;
			memset(item->mRelativePath, 0, 256);
			strcpy(item->mRelativePath, path);
			item->mLastWriteTime=fileinfo.time_write;
		}
		_findclose(hFile);
#else
#endif
		return item;
	}
}
#endif
