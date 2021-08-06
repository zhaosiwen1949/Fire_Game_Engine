#include "ExportedImageResource.h"
#include "Runtime/Graphics/ImageUtils.h"
namespace Alice{
	int ExportedImageResource::API_GetColor(lua_State*L) {
		ExportedImageResource*ptr = TO_USERDATA(L, ExportedImageResource, 1);
		int x = lua_tointeger(L, 2);
		int y = lua_tointeger(L, 3);
        x=x<0?0:x;
        y=y<0?0:y;
        int r=0,g=0,b=0,a=0;
		if (ptr->mBPP == 8) {
			int nIndex = x + y*ptr->mWidth;
			if (nIndex < ptr->mPixelData.mDataLen) {
                r=ptr->mPixelData.mData[nIndex];
                g=r;
                b=r;
                a=255;
			}
		}else if (ptr->mBPP == 24) {
			int nIndex = 3 * (x + y*ptr->mWidth);
			if (nIndex+2 < ptr->mPixelData.mDataLen) {
                r=ptr->mPixelData.mData[nIndex];
                g=ptr->mPixelData.mData[nIndex + 1];
                b=ptr->mPixelData.mData[nIndex + 2];
                a=255;
			}
		}else if (ptr->mBPP == 32) {
			int nIndex = 4 * (x + y*ptr->mWidth);
			if (nIndex+3 < ptr->mPixelData.mDataLen) {
                r=ptr->mPixelData.mData[nIndex];
                g=ptr->mPixelData.mData[nIndex + 1];
                b=ptr->mPixelData.mData[nIndex + 2];
                a=ptr->mPixelData.mData[nIndex + 3];
			}
		}
        lua_pushinteger(L, r);
        lua_pushinteger(L, g);
        lua_pushinteger(L, b);
        lua_pushinteger(L, a);
		return 4;
	}
	int ExportedImageResource::API_GetSize(lua_State*L) {
		ExportedImageResource*ptr = TO_USERDATA(L, ExportedImageResource, 1);
		lua_pushinteger(L, ptr->mWidth);
		lua_pushinteger(L, ptr->mHeight);
		return 2;
	}
	int ExportedImageResource::API_GetPath(lua_State*L) {
		ExportedImageResource*ptr = TO_USERDATA(L, ExportedImageResource, 1);
		lua_pushstring(L, ptr->mName.mText);
		return 1;
	}
	int ExportedImageResource::API_Load(lua_State*L) {
		const char * path = lua_tostring(L, 1);
		ImageResource*ret = ImageResource::Init(path);
		if (ret != nullptr) {
			CreateLuaDataViaExistObject(ret, ImageResource, L);
			ret->PushUserData(L);
		}else {
			lua_pushnil(L);
		}
		return 1;
	}
	int ExportedImageResource::API_Unload(lua_State*L) {
		ExportedImageResource*ptr = TO_USERDATA(L, ExportedImageResource, 1);
		delete ptr;
		return 0;
	}
	int ExportedImageResource::API_SetFileData(lua_State*L) {
		ExportedImageResource*ptr = TO_USERDATA(L, ExportedImageResource, 1);
		unsigned char *fileData = (unsigned char*)lua_tostring(L, 2);
		int dataLen = lua_tointeger(L, 3);
		ptr->InitWithRawImageFileData(fileData, dataLen);
		return 0;
	}
	int ExportedImageResource::API_SetImageData(lua_State*L) {
		ExportedImageResource*ptr = TO_USERDATA(L, ExportedImageResource, 1);
		unsigned char *pixelData = (unsigned char*)lua_tostring(L, 2);
		int dataLen = lua_tointeger(L, 3);
		ptr->mPixelData.SetBufferSize(dataLen);
		memcpy(ptr->mPixelData.mData, pixelData, dataLen);
		ptr->mPixelData.mDataLen = dataLen;
		return 0;
	}
	int ExportedImageResource::API_SetImageInfo(lua_State*L) {
		ExportedImageResource*ptr = TO_USERDATA(L, ExportedImageResource, 1);
		ptr->mWidth = lua_tointeger(L, 2);
		ptr->mHeight = lua_tointeger(L, 3);
		ptr->mBPP = lua_tointeger(L, 4);
		return 0;
	}
	int ExportedImageResource::API_GetImageData(lua_State*L) {
		ExportedImageResource*ptr = TO_USERDATA(L, ExportedImageResource, 1);
		lua_pushlstring(L, (char*)ptr->mPixelData.mData,ptr->mPixelData.mDataLen);
		lua_pushinteger(L, ptr->mPixelData.mDataLen);
		return 2;
	}
	int ExportedImageResource::API_GetImageInfo(lua_State*L) {
		ExportedImageResource*ptr = TO_USERDATA(L, ExportedImageResource, 1);
		lua_pushinteger(L, ptr->mWidth);
		lua_pushinteger(L, ptr->mHeight);
		lua_pushinteger(L, ptr->mBPP);
		return 3;
	}
	int ExportedImageResource::API_Scale(lua_State*L) {
		const char *srcimage = lua_tostring(L,1);
		const char *dstimage = lua_tostring(L, 2);
		int target_width = lua_tointeger(L, 3);
		int target_height = lua_tointeger(L, 4);
		FREE_IMAGE_FILTER option = FREE_IMAGE_FILTER::FILTER_CATMULLROM;
		if (lua_gettop(L)==5){
			option = (FREE_IMAGE_FILTER)lua_tointeger(L, 5);
		}
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		fif = FreeImage_GetFileType(srcimage);
		if (fif == FIF_UNKNOWN) {
			lua_pushboolean(L, false);
			return 1;
		}
		FIBITMAP*srcImage = FreeImage_Load(fif, srcimage);
		FIBITMAP*scaledImage = FreeImage_Rescale(srcImage, target_width, target_height, option);
		if (scaledImage == NULL) {
			lua_pushboolean(L, false);
			return 1;
		}
		FreeImage_Save(fif, scaledImage, dstimage);
		FreeImage_Unload(scaledImage);
		FreeImage_Unload(srcImage);
		lua_pushboolean(L, true);
		return 1;
	}
	int ExportedImageResource::API_Save(lua_State*L) {
		ExportedImageResource*ptr = TO_USERDATA(L, ExportedImageResource, 1);
		ptr->Save(lua_tostring(L, 2));
		return 0;
	}
	int ExportedImageResource::Export(lua_State*L) {
		luaL_Reg apis[] = {
			EXPORT_API(SetFileData)
			EXPORT_API(SetImageData)
			EXPORT_API(GetImageData)
			EXPORT_API(SetImageInfo)
			EXPORT_API(GetImageInfo)
			EXPORT_API(Scale)
			EXPORT_API(Save)
			EXPORT_API(GetColor)
			EXPORT_API(GetSize)
			EXPORT_API(GetPath)
			EXPORT_API(Unload)
			EXPORT_API(ClassID)
			{NULL,NULL}
		};
		NEW_LUA_TYPE_WITH_API(L, ImageResource, apis);
		luaL_Reg api_c[] =
		{
			{ "New",New_ExportedAliceType },
			EXPORT_API(Load)
			EXPORT_API(Scale)
			{NULL,NULL}
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, ImageResource, api_c);
		return 0;
	}
}
