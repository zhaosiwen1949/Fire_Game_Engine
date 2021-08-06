#include "FolderSelecter.h"
#include "Runtime/String/StringUtils.h"

namespace Editor
{
	bool FolderSelecter::SetFolder(const char*tip, char*targetFolder)
	{
		TCHAR user_tip[256] = { 0 };
		TCHAR selected_folder[256] = { 0 };
		MultiByteToWideChar(CP_UTF8, 0, tip, -1, user_tip, 200);
		BROWSEINFO bInfo = { 0 };
		bInfo.hwndOwner = GetForegroundWindow();//父窗口  
		bInfo.lpszTitle = user_tip;
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI//包含一个编辑框 用户可以手动填写路径 对话框可以调整大小之类的. |
			| BIF_UAHINT;//带TIPS提示//关于更多的 ulFlags 参考 http://msdn.microsoft.com/en-us/library/bb773205(v=vs.85).aspx  
		LPITEMIDLIST lpDlist;
		lpDlist = SHBrowseForFolder(&bInfo);
		if (lpDlist != NULL)//单击了确定按钮  
		{
			SHGetPathFromIDList(lpDlist, selected_folder);
			Alice::StringUtils::UnicodeToASCII(selected_folder, targetFolder,256);
			Alice::StringUtils::ToSTDPath(targetFolder);
			return true;
		}
		return false;
	}
	bool FolderSelecter::SelectFolder(const char*tip, char*targetFolder){
		TCHAR user_tip[256] = { 0 };
		TCHAR selected_folder[256] = { 0 };
		MultiByteToWideChar(CP_UTF8, 0, tip, -1, user_tip, 200);
		BROWSEINFO bInfo = { 0 };
		bInfo.hwndOwner = GetForegroundWindow();//父窗口  
		bInfo.lpszTitle = user_tip;
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI//包含一个编辑框 用户可以手动填写路径 对话框可以调整大小之类的. |
			| BIF_UAHINT;//带TIPS提示//关于更多的 ulFlags 参考 http://msdn.microsoft.com/en-us/library/bb773205(v=vs.85).aspx  
		LPITEMIDLIST lpDlist;
		lpDlist = SHBrowseForFolder(&bInfo);
		if (lpDlist != NULL)//单击了确定按钮  
		{
			SHGetPathFromIDList(lpDlist, selected_folder);
			Alice::StringUtils::UnicodeToASCII(selected_folder, targetFolder, 256);
			Alice::StringUtils::ToSTDPath(targetFolder);
			return true;
		}
		return false;
	}
	bool FolderSelecter::SetFolderT(LPCTSTR tip, LPTSTR targetFolder)
	{
		BROWSEINFO bInfo = { 0 };
		bInfo.hwndOwner = GetForegroundWindow();//父窗口  
		bInfo.lpszTitle = tip;
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI//包含一个编辑框 用户可以手动填写路径 对话框可以调整大小之类的. |
			| BIF_UAHINT;//带TIPS提示//关于更多的 ulFlags 参考 http://msdn.microsoft.com/en-us/library/bb773205(v=vs.85).aspx  
		LPITEMIDLIST lpDlist;
		lpDlist = SHBrowseForFolder(&bInfo);
		if (lpDlist != NULL){
			SHGetPathFromIDList(lpDlist, targetFolder);
			return true;
		}
		return false;
	}

	bool FolderSelecter::SetFolderWithOutNew(const char*tip, char*targetFolder)
	{
		BROWSEINFOA bInfo = { 0 };
		bInfo.hwndOwner = GetForegroundWindow();//父窗口  
		bInfo.lpszTitle = tip;
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI//包含一个编辑框 用户可以手动填写路径 对话框可以调整大小之类的. |
			| BIF_UAHINT//带TIPS提示
			| BIF_NONEWFOLDERBUTTON;//不带新建文件夹按钮;
									//关于更多的 ulFlags 参考 http://msdn.microsoft.com/en-us/library/bb773205(v=vs.85).aspx  
		LPITEMIDLIST lpDlist;
		lpDlist = SHBrowseForFolderA(&bInfo);
		if (lpDlist != NULL)
		{
			SHGetPathFromIDListA(lpDlist, targetFolder);
			Alice::StringUtils::ToSTDPath(targetFolder);
			return true;
		}
		return false;
	}
}