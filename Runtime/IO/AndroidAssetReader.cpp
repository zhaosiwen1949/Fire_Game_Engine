#include "AndroidAssetReader.h"
#if ALICE_ANDROID
#include "Runtime/Debugger/Log.h"
namespace Alice{
	AAssetManager*AndroidAssetReader::sAssetManager = nullptr;
	bool AndroidAssetReader::LoadInternalData(const char*path, Data &data){
		AAsset * pAsset = AAssetManager_open(sAssetManager, path, AASSET_MODE_UNKNOWN);
		if (pAsset == NULL){
			return false;
		}
		size_t size = AAsset_getLength(pAsset);
		if (size > 0){
			data.SetBufferSize(size+1);
			int iRet = AAsset_read(pAsset, data.mData, size);
			if (iRet <= 0){
				return false;
			}
			data.mDataLen = size;
		}
		AAsset_close(pAsset);
		return true;
	}

	bool AndroidAssetReader::DeleteFileWithPath(const char *path) {
		remove(path);
		return true;
	}

	bool AndroidAssetReader::DeleteDir(const char *path) {
		DIR* pDir = opendir(path);
		if (pDir ==nullptr){
			return false;
		}
		char szFullPath[256];
		struct dirent* diritem;
		while ((diritem = readdir(pDir)) != NULL) {
			if (strcmp(diritem->d_name, "..") == 0 || strcmp(diritem->d_name, ".") == 0)
				continue;
			memset(szFullPath, 0, 256);
			strcpy(szFullPath, path);
			strcat(szFullPath, diritem->d_name);
			struct stat info;
			stat(szFullPath, &info);
			if (S_ISDIR(info.st_mode)) {
				DeleteDir(szFullPath);
			}else {
				remove(szFullPath);
			}
		}
		rmdir(path);
		closedir(pDir);
		return true;
	}

	void AndroidAssetReader::GetItemListInDirectory(lua_State*L, const char * path) {
		DIR *pDir = opendir(path);
		if (pDir==nullptr){
			return ;
		}
		struct dirent *dmsg;
		int nIndex = 1;
		char szFullPath[256];
		while ((dmsg = readdir(pDir)) != NULL)
		{
			if (strcmp(dmsg->d_name, ".") != 0 && strcmp(dmsg->d_name, "..") != 0){
				lua_pushinteger(L, nIndex++);
				lua_newtable(L);
				lua_pushstring(L, dmsg->d_name);
				lua_setfield(L, -2, "name");
				memset(szFullPath, 0, 256);
				strcpy(szFullPath, path);
				strcat(szFullPath, dmsg->d_name);
				struct stat info;
				stat(szFullPath, &info);
				if (S_ISDIR(info.st_mode)) {
					lua_pushboolean(L, true);
				}else{
					lua_pushboolean(L, false);
				}
				lua_setfield(L, -2, "is_dir");
				lua_settable(L, -3);
			}
		}
		closedir(pDir);
	}
}
#endif