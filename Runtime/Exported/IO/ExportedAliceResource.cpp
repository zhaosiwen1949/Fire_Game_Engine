#include "ExportedAliceResource.h"

namespace Alice
{

	int ExportedAliceResource::API_Unload(lua_State*L)
	{
		ExportedAliceResource*ptr = TO_USERDATA(L, ExportedAliceResource, 1);
		ptr->Unload();
		return 0;
	}

	int ExportedAliceResource::API_Dump(lua_State*L)
	{
		Info("AliceResource::Dump---start");
		for (auto iter=mCachedResources.begin();iter!=mCachedResources.end();++iter){
			Debug("%s",iter->first.c_str());
		}
		Info("AliceResource::Dump---end");
		return 0;
	}
	int ExportedAliceResource::API_GetData(lua_State*L)
	{
		lua_pushnil(L);
		return 1;
	}
	int ExportedAliceResource::API_GetType(lua_State*L)
	{
		if (lua_gettop(L) != 1)
		{
			lua_pushnil(L);
			return 1;
		}
		ExportedAliceResource*ptr = TO_USERDATA(L, ExportedAliceResource, 1);
		switch (ptr->mType)
		{
		case ResourceTypeMaterial:
			lua_pushstring(L, "Material");
			break;
		case ResourceTypeShader:
			lua_pushstring(L, "Shader");
			break;
		case ResourceTypePVRTCRGBA:
		case ResourceTypeETC1RGB:
		case ResourceTypePNGImage:
			lua_pushstring(L, "Texture");
			break;
		default:
			lua_pushstring(L, "unknown");
			break;
		}
		return 1;
	}

	int ExportedAliceResource::API_Add(lua_State*L)
	{
		AddResource(lua_tostring(L, 1), lua_tostring(L, 2), lua_tostring(L, 3));
		return 0;
	}

	int ExportedAliceResource::API_Remove(lua_State*L)
	{
		RemoveResource(lua_tostring(L, 1), lua_tostring(L, 2));
		return 0;
	}

	int ExportedAliceResource::API_GetRes(lua_State*L)
	{
		const char*name = lua_tostring(L,1);
		const char*guid=GetResourceGUIDViaUserName(name);
		if (guid!=nullptr){
			lua_pushstring(L, guid);
		}else{
			lua_pushnil(L);
		}
		return 1;
	}

	int ExportedAliceResource::API_LoadByte(lua_State*L)
	{
		const char*path = lua_tostring(L, 1);
		Data data;
		if (ResourceManager::LoadData(path,data)){
			lua_pushlstring(L, (char*)data.mData, data.mDataLen);
		}else{
            Error("Resource.Loadbyte fail %s",path);
			lua_pushnil(L);
		}
		return 1;
	}

	//load from Assets/...
	int ExportedAliceResource::API_Load(lua_State*L)
	{
		const char*path = lua_tostring(L,1);
		std::unordered_map<std::string, Resource*>::iterator iter = mCachedResources.find(path);
		if (iter != mCachedResources.end()){
			iter->second->PushUserData(L);
			return 1;
		}
		Resource*ret = LoadResource<Resource>(path);
		if (ret != nullptr){
			CreateLuaDataViaExistObject(ret, Resource, L);
			ret->PushUserData(L);
		}else{
			lua_pushnil(L);
		}
		return 1;
	}

	int ExportedAliceResource::API_UnloadUnusedResources(lua_State*L)
	{
		return 0;
	}

	int ExportedAliceResource::Export(lua_State*L)
	{
		Init();
		luaL_Reg apis[] = {
			EXPORT_API(GetData)
			EXPORT_API(GetType)
			EXPORT_API(Unload)
			EXPORT_API(ClassID)
			{ NULL,NULL }
		};
		NEW_LUA_TYPE_WITH_API(L, Resource, apis);
		luaL_Reg api_c[] = 
		{
			EXPORT_API(ClassID)
			EXPORT_API(Add)
			EXPORT_API(Remove)
			EXPORT_API(Load)
			EXPORT_API(LoadByte)
			EXPORT_API(GetRes)
			EXPORT_API(Dump)
			EXPORT_API(UnloadUnusedResources)
			{ NULL,NULL }
		};
		EXPORT_ALICE_PACKAGE_TO_SCRIPT(L, Resource, api_c);
		return 0;
	}
}
