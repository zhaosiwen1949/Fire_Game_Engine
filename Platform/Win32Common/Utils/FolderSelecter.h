#pragma once
#include "Runtime/RuntimePrefix.h"
namespace Editor
{
	class FolderSelecter
	{
	public:
		static bool SetFolderT(LPCTSTR tip, LPTSTR targetFolder);
		static bool SetFolder(const char*tip, char*targetFolder);
		static bool SelectFolder(const char * tip, char*targetFolder);
		static bool SetFolderWithOutNew(const char*tip, char*targetFolder);
	};
}