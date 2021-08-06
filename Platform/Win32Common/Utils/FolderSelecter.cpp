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
		bInfo.hwndOwner = GetForegroundWindow();//������  
		bInfo.lpszTitle = user_tip;
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI//����һ���༭�� �û������ֶ���д·�� �Ի�����Ե�����С֮���. |
			| BIF_UAHINT;//��TIPS��ʾ//���ڸ���� ulFlags �ο� http://msdn.microsoft.com/en-us/library/bb773205(v=vs.85).aspx  
		LPITEMIDLIST lpDlist;
		lpDlist = SHBrowseForFolder(&bInfo);
		if (lpDlist != NULL)//������ȷ����ť  
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
		bInfo.hwndOwner = GetForegroundWindow();//������  
		bInfo.lpszTitle = user_tip;
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI//����һ���༭�� �û������ֶ���д·�� �Ի�����Ե�����С֮���. |
			| BIF_UAHINT;//��TIPS��ʾ//���ڸ���� ulFlags �ο� http://msdn.microsoft.com/en-us/library/bb773205(v=vs.85).aspx  
		LPITEMIDLIST lpDlist;
		lpDlist = SHBrowseForFolder(&bInfo);
		if (lpDlist != NULL)//������ȷ����ť  
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
		bInfo.hwndOwner = GetForegroundWindow();//������  
		bInfo.lpszTitle = tip;
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI//����һ���༭�� �û������ֶ���д·�� �Ի�����Ե�����С֮���. |
			| BIF_UAHINT;//��TIPS��ʾ//���ڸ���� ulFlags �ο� http://msdn.microsoft.com/en-us/library/bb773205(v=vs.85).aspx  
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
		bInfo.hwndOwner = GetForegroundWindow();//������  
		bInfo.lpszTitle = tip;
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI//����һ���༭�� �û������ֶ���д·�� �Ի�����Ե�����С֮���. |
			| BIF_UAHINT//��TIPS��ʾ
			| BIF_NONEWFOLDERBUTTON;//�����½��ļ��а�ť;
									//���ڸ���� ulFlags �ο� http://msdn.microsoft.com/en-us/library/bb773205(v=vs.85).aspx  
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