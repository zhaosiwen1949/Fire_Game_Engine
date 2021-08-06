#pragma once

namespace Alice
{
#if ALICE_ANDROID
#elif ALICE_EDITOR
	class FileItem
	{
	public:
		char mRelativePath[256];
		__int64 mLastWriteTime;
		static FileItem* Get(const char*path);
	};
#endif
}